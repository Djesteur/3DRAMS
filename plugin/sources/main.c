#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#include <3ds.h>

#include "plgldr.h"
#include "csvc.h"
#include "common.h"

#include "Request.h"
#include "RequestExecutor.h"

static PluginMenu   menu;
static Handle       thread;
static Handle       onProcessExitEvent, resumeExitEvent;
static u8           stack[STACK_SIZE] ALIGN(8);

#define SOC_BUFFERSIZE  0x010000

static u32 *g_socContext = NULL;
void* __service_ptr;


// Add an entry in the menu
void    NewEntry(const char *name, const char *hint)
{
    if (menu.nbItems >= MAX_ITEMS_COUNT)
        return;

    u32 index = menu.nbItems;

    menu.states[index] = 0;
    if (name)
        strncpy(menu.items[index], name, MAX_BUFFER);
    if (hint)
        strncpy(menu.hints[index], hint, MAX_BUFFER);
    ++menu.nbItems;
}

// Create the menu
void InitMenu(void) {

    memset(&menu, 0, sizeof(menu));
    strncpy(menu.title, "3DRAMS plugin", MAX_BUFFER);
    NewEntry("Activate connection to 3DRAMS.", "Start communications with 3DRAMS.");
}

bool InitTCP(int *listenSocket) {

    char *msgTitle, *msgBody;

    // Now initialise soc:u service
    int result = socInit(g_socContext, SOC_BUFFERSIZE);

    if (result != 0) {

        msgTitle = "SocInit error:";
        msgBody = "Error with the socket function initialisation.";
        PLGLDR__DisplayErrMessage(msgTitle, msgBody, result);
        return false;
    }

    *listenSocket = socket(AF_INET, SOCK_STREAM, 0);


    if(*listenSocket < 0) {

        msgTitle = "Socket error:";
        msgBody = strerror(errno);
        PLGLDR__DisplayErrMessage(msgTitle, msgBody, errno);
    }

    struct sockaddr_in addr;

    addr.sin_family = AF_INET;
    addr.sin_port = htons(10700);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    
    result = bind(*listenSocket, (struct sockaddr*) &addr, sizeof(struct sockaddr_in));

    if(result != 0) {

        msgTitle = "Bind error:";
        msgBody = strerror(errno);
        PLGLDR__DisplayErrMessage(msgTitle, msgBody, errno);
        return false;  
    }


    result = listen(*listenSocket, 1);

    if(result != 0) {

        msgTitle = "Listen error:";
        msgBody = strerror(errno);
        PLGLDR__DisplayErrMessage(msgTitle, msgBody, errno);
        return false;
    }

    return true;
}

Result SocketContextInit() {

    PluginHeader *header = (PluginHeader *)0x07000000;
    return svcControlMemory((u32 *)&g_socContext, header->heapVA + header->heapSize, 0, SOC_BUFFERSIZE, MEMOP_ALLOC, MEMPERM_READ | MEMPERM_WRITE);
}

Result SocketContextExit() {

    PluginHeader *header = (PluginHeader *)0x07000000;
    return svcControlMemory((u32 *)&g_socContext, header->heapVA + header->heapSize, 0, SOC_BUFFERSIZE, MEMOP_FREE, MEMPERM_DONTCARE);
}

void Flash(u32 color) {

    color |= 0x01000000;
    for (u32 i = 0; i < 64; i++) {

        REG32(0x10202204) = color;
        svcSleepThread(10000000);
    }

    REG32(0x10202204) = 0;
}

int nbReceive = 0;

void communicateWith3DRAMS(const int socket, fd_set *listenSet, struct timeval *timeout, bool *connected) {

    FD_ZERO(listenSet);
    FD_SET(socket, listenSet);

    select(socket+1, listenSet, NULL, NULL, timeout);

    if(FD_ISSET(socket, listenSet)) {

        char *message = (char*) malloc(MAX_REQUEST_SIZE*sizeof(char));

        char *msgTitle, *msgBody;

        int communicationResult = recv(socket, message, MAX_REQUEST_SIZE, 0);

        if(communicationResult == 0 || errno == 0x68) { 

            *connected = false; 
            close(socket);
        }

        else if (communicationResult < 0) {

            char *msgTitle = "Communication error:";
            char *msgBody = strerror(errno);
            PLGLDR__DisplayErrMessage(msgTitle, msgBody, errno);
            *connected = false; 
            close(socket);
        }

        else {

            nbReceive++;

            Request *result = executeRequest(message);

            if(result == NULL) {

                char *msgTitle = "Request result:";
                char *msgBody = "Returning NULL (unknown request)";
                PLGLDR__DisplayMessage(msgTitle, msgBody);
                //*connected = false; 
                //close(socket);
            }

            else {

                char *reply = (char*) malloc(MAX_REQUEST_SIZE*sizeof(char));

                uint16_t messageSize = requestToString(result, reply);
                communicationResult = send(socket, reply, messageSize, 0);

                //Special case
                if(result->type == Disconnect) {

                    *connected = false; 
                    close(socket);
                }

                freeRequest(result);
                free(reply);
            }
        }

        free(message);
        usleep(100000);
    }
}

// Plugin main thread entrypoint
void ThreadMain(void *arg) {

    SocketContextInit(); 
    InitMenu();

    //For game freezing
    
    int clientSocket = -1, listenSocket = -1;
    int selectResult = 0, communicationResult = 0;
    bool haveToStop = !InitTCP(&listenSocket), connectedTo3DRAMS = false;
    char *msgTitle, *msgBody;
    struct timeval timeout = {0, 100000};

    unsigned int currentTime = 0;
    u32 currentColor;

    fd_set communicationFD;
    FD_ZERO(&communicationFD);
    FD_SET(listenSocket, &communicationFD);

    while(!haveToStop)  {

        if(svcWaitSynchronization(onProcessExitEvent, 1000000) != 0x09401BFE)  { haveToStop = true;  }

        if(HID_PAD & BUTTON_SELECT) { PLGLDR__DisplayMenu(&menu); }

        if(!connectedTo3DRAMS && menu.states[0]) {

            select(listenSocket+1, &communicationFD, NULL, NULL, &timeout);
            
            if(FD_ISSET(listenSocket, &communicationFD)) {

                clientSocket = accept(listenSocket, NULL, 0);
                FD_ZERO(&communicationFD);
                FD_SET(clientSocket, &communicationFD);
                connectedTo3DRAMS = true;

                msgTitle = "Connected.";
                msgBody =  "Connected to 3DRAMS.";
                PLGLDR__DisplayMessage(msgTitle, msgBody);
            }
        }

        if(connectedTo3DRAMS) { 

            communicateWith3DRAMS(clientSocket, &communicationFD, &timeout, &connectedTo3DRAMS);

            if(!connectedTo3DRAMS) {

                FD_ZERO(&communicationFD);
                FD_SET(listenSocket, &communicationFD);

                msgTitle = "Disconnected.";
                msgBody =  "Disconnected from 3DRAMS.";
                PLGLDR__DisplayMessage(msgTitle, msgBody);
            } 
        }

        //ApplyCheats();
    }

    socExit();
    SocketContextExit();

    plgLdrExit();
    srvExit();

    svcSignalEvent(resumeExitEvent);
    svcExitThread();
}

extern char* fake_heap_start;
extern char* fake_heap_end;
extern u32 __ctru_heap;
extern u32 __ctru_linear_heap;

u32 __ctru_heap_size        = 0;
u32 __ctru_linear_heap_size = 0;

void    __system_allocateHeaps(PluginHeader *header)
{
    __ctru_heap_size = header->heapSize;
    __ctru_heap = header->heapVA;

    // Set up newlib heap
    fake_heap_start = (char *)__ctru_heap;
    fake_heap_end = fake_heap_start + __ctru_heap_size;
}

// Entrypoint, game will starts when you exit this function
void    main(void)
{
    PluginHeader *header = (PluginHeader *)0x07000000;

    if (header->magic != HeaderMagic)
        return; ///< Abort plugin as something went wrong

    // Init heap
    __system_allocateHeaps(header);

    // Init services
    srvInit();
    plgLdrInit();

    // Get the event triggered  when the game will exit
    svcControlProcess(CUR_PROCESS_HANDLE, PROCESSOP_GET_ON_EXIT_EVENT, (u32)&onProcessExitEvent, (u32)&resumeExitEvent);

    // Create the plugin's main thread
    svcCreateThread(&thread, ThreadMain, 0, (u32 *)(stack + STACK_SIZE), 30, -1);
}
