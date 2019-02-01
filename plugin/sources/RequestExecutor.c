#include "RequestExecutor.h"

Request* executeRequest(const char *msg) {

    Request *newRequest = stringToRequest(msg), *result = NULL;

    switch(newRequest->type) {

       case Connect:
            result = connectRequest(newRequest);
            break;

        case Disconnect:
        	result = disconnectRequest(newRequest);
            break;

        case GetValue:
        	result = getValueRequest(newRequest);
            break;
                
        case GetRangeOfValue:
        	result = getRangeOfValueRequest(newRequest);
            break;

        case SetValue:
        	result = setValueRequest(newRequest);
            break;

        case SendCheatCode:
        	result = sendCheatCodeRequest(newRequest);
            break;

        case DeleteCheatCode:
        	result = deleteCheatCodeRequest(newRequest);
            break;

        case ChangeEnableCheatCode:
        	result = changeEnableCheatCodeRequest(newRequest);
            break;

        case GetSetOfValue:
        	result = getSetOfValueRequest(newRequest);
            break;

        case FreezeGame:
            result = freezeGameRequest(newRequest);
            break;

        //Return null if unknown request
    }

    freeRequest(newRequest);

    return result;
}

Request* connectRequest(Request *request) {

    Request *newResult = newRequest(request->requester, request->type);
    writeByte(newResult, 0);

    return newResult;
}

Request* disconnectRequest(Request *request) {

    Request *newResult = newRequest(request->requester, request->type);
    writeByte(newResult, 0);

    return newResult;
}

Request* getValueRequest(Request *request) {

    uint32_t *askedNumber;
    askedNumber = (uint32_t*)(parametersToNumber(&(request->parameters[0])));

    Request *newResult = newRequest(request->requester, request->type);
    writeNumber(newResult, *askedNumber);

    return newResult;
}

Request* getRangeOfValueRequest(Request *request) {

    return newRequest(request->requester, request->type);
}

Request* setValueRequest(Request *request) {

    return newRequest(0, 0);
}

Request* sendCheatCodeRequest(Request *request) {

    return newRequest(0, 0);
}

Request* deleteCheatCodeRequest(Request *request) {

    return newRequest(0, 0);
}

Request* changeEnableCheatCodeRequest(Request *request) {

    return newRequest(0, 0);
}

Request* getSetOfValueRequest(Request *request) {

    Request *newResult = newRequest(request->requester, request->type);
    newResult->nbParameters = 0;

    uint8_t nbWorkingBytes = request->parameters[0];
    uint16_t currentAddress = 1;
    uint32_t *askedNumber = NULL;
    uint32_t askedValue = 0;

    while(currentAddress < request->nbParameters) {

        askedNumber = (uint32_t*)(parametersToNumber(&(request->parameters[currentAddress])));
        writeNumber(newResult, (uint32_t)askedNumber);

        askedValue = *askedNumber;

        if(nbWorkingBytes == 1) { 

            askedValue <<= 24;
            askedValue >>= 24;
            writeByte(newResult, (uint8_t)askedValue); 
        }

        else if(nbWorkingBytes == 2) {

            askedValue <<= 16;
            askedValue >>= 16;
            //writeHalfNumber(newResult, (uint16_t)askedValue); Fonction à faire
        }

        else if(nbWorkingBytes == 4) { writeNumber(newResult, askedValue); }

        currentAddress += nbWorkingBytes;
    }

    return newResult;
}

Request* freezeGameRequest(Request *request) {

    // 0 -> game is not freeze
    // else -> game is freezing

    u32 *tls = (u32 *)getThreadLocalStorage();
    *tls = THREADVARS_MAGIC;

    Request *newResult = newRequest(request->requester, request->type);

    uint32_t askingFreeze = parametersToNumber(&(request->parameters[0]));

    if(askingFreeze == 0) {

        UnfreezeGameFunction();
        writeByte(newResult, 0);
    }

    else {

        FreezeGameFunction();
        writeByte(newResult, 1);
    }

    return newResult;
}


//By Nanquitas

static bool ThreadPredicate(u32 *kthread) {

    // Check that the thread is not part of our plugin
    u32 *tls = (u32 *)kthread[0x25];
    return *tls != THREADVARS_MAGIC;
}

void FreezeGameFunction() { svcControlProcess(CUR_PROCESS_HANDLE, PROCESSOP_SCHEDULE_THREADS, 1, (u32)ThreadPredicate); }
void UnfreezeGameFunction() { svcControlProcess(CUR_PROCESS_HANDLE, PROCESSOP_SCHEDULE_THREADS, 0, (u32)ThreadPredicate); }