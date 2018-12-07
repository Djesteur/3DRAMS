#include "RequestExecutor.h"

Request* executeRequest(const char *msg) {

    Request *newRequest = stringToRequest(msg), *result;

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

    uint32_t *getValueOfNumber;
    getValueOfNumber = (uint32_t*)(parametersToNumber(&(request->parameters[0])));

    Request *newResult = newRequest(request->requester, request->type);
    writeNumber(newResult, *getValueOfNumber);

    return newResult;
}

Request* getRangeOfValueRequest(Request *request) {

    return newRequest(0, 0);
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

    return newRequest(0, 0);
}
