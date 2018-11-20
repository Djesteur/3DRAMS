#include <string.h>
#include <malloc.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <3ds.h>

#include "plgldr.h"
#include "csvc.h"
#include "common.h"

static PluginMenu   menu;
static Handle       thread;
static Handle       onProcessExitEvent, resumeExitEvent;
static u8           stack[STACK_SIZE] ALIGN(8);

#define SOC_ALIGN       0x1000
#define SOC_BUFFERSIZE  0x100000 // Used in 3ds example in the github of devkitARM

void* __service_ptr;
static u32 *g_socContext = NULL;



Result SocketContextInit() {

    PluginHeader *header = (PluginHeader *)0x07000000;
    return svcControlMemory((u32 *)&g_socContext, header->heapVA + header->heapSize, 0, SOC_BUFFERSIZE, MEMOP_ALLOC, MEMPERM_READ | MEMPERM_WRITE);
}

Result SocketContextExit() {

    PluginHeader *header = (PluginHeader *)0x07000000;
    return svcControlMemory((u32 *)&g_socContext, header->heapVA + header->heapSize, 0, SOC_BUFFERSIZE, MEMOP_FREE, MEMPERM_DONTCARE);
}



void NewEntry(const char *name, const char *hint) {

    if (menu.nbItems >= MAX_ITEMS_COUNT) { return; }

    u32 index = menu.nbItems;

    menu.states[index] = 0;

    if(name) { strncpy(menu.items[index], name, MAX_BUFFER); }
    if(hint) { strncpy(menu.hints[index], hint, MAX_BUFFER); }

    ++menu.nbItems;
}



void InitMenu() {

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

/*To do:

    - Add real error codes

*/

// Plugin main thread entrypoint
void ThreadMain(void *arg) {

    SocketContextInit(); 
    InitMenu();

    int listenSocket = 0, clientSocket = -1;
    bool connectedTo3DRAMS = false;
    bool haveToStop = !InitTCP(&listenSocket); //Call socInit

    char *msgTitle, *msgBody;

    //For listening connection

    int selectResult = 0;
    int communicationResult = 0;

    fd_set communicationFD;

    FD_SET(listenSocket, &communicationFD);
    

    //For both

    struct timeval timeout = {0, 1000};

    while(!haveToStop)  {

        if(svcWaitSynchronization(onProcessExitEvent, 1000000) != 0x09401BFE)  { haveToStop = true;  }

        //Display main menu
        if(HID_PAD & BUTTON_SELECT) { PLGLDR__DisplayMenu(&menu); }

        if(!connectedTo3DRAMS && menu.states[0]) {

            select(listenSocket, &communicationFD, NULL, NULL, &timeout);
            
            if(FD_ISSET(listenSocket, &communicationFD)) {

                clientSocket = accept(listenSocket, NULL, 0);
                FD_CLR(listenSocket, &communicationFD);
                FD_SET(clientSocket, &communicationFD);
                connectedTo3DRAMS = true;

                msgTitle = "Connected !";
                msgBody =  "Connected to 3DRAMS.";
                PLGLDR__DisplayMessage(msgTitle, msgBody);
            }
        }

        if(connectedTo3DRAMS) {

            select(clientSocket, &communicationFD, NULL, NULL, &timeout);

            if(FD_ISSET(clientSocket, &communicationFD)) {

                msgTitle = "Message from 3DRAMS";
                communicationResult = recv(clientSocket, msgBody, 32, 0);

                if(communicationResult == 0) { //Disconnected

                    msgBody = "Disconnected";
                    connectedTo3DRAMS = false;
                    FD_CLR(clientSocket, &communicationFD);
                    FD_SET(listenSocket, &communicationFD);
                }
                
                PLGLDR__DisplayMessage(msgTitle, msgBody);
            }
        }


        //CommunicateWith3DRAMS();
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
