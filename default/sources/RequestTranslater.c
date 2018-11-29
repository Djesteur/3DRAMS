#include "RequestTranslater.h"

uint16_t requestToString(const Request request, char *msg) {

    hasValidNumberOfParameters(request);

    msg[0] = (char)(request.requester);
    msg[1] = (char)(request.type);

    uint16_t splitedSize = (uint16_t)(request.nbParameters);
    splitedSize >>= 8;
    msg[2] = (char)(splitedSize);
    msg[3] = (char)(request.nbParameters);

    for(uint16_t i = 0; i < request.nbParameters; i++) { msg[4+i] = (char)(request.parameters[i]); }

    return 4 + request.nbParameters;
}

Request stringToRequest(const char *msg, const uint16_t msgSize) {

    Request finalRequest;

    if(msgSize < 4) { return finalRequest; }

    finalRequest.requester = (enum Module)msg[0];
    finalRequest.type = (enum RequestType)msg[1];

    finalRequest.nbParameters = (uint16_t)msg[2];
    finalRequest.nbParameters <<= 8;
    finalRequest.nbParameters += (uint16_t)msg[3];
    finalRequest.parameters = (uint8_t*) malloc(sizeof(uint8_t)*MAX_MESSAGE_SIZE);

    for(uint16_t i = 4; i < finalRequest.nbParameters && i < msgSize; i++) { finalRequest.parameters[i] = (uint8_t)msg[i];  }
    
    hasValidNumberOfParameters(finalRequest);

    return finalRequest;
}

void freeRequestParameters(Request request) {

    if(request.parameters != NULL) { free(request.parameters); }
}


bool hasValidNumberOfParameters(const Request request) {

    unsigned int neededParameters = 0;
    switch(request.type) {

        case Disconnect:
        case GetValue:
        case DeleteCheatCode:
        case ChangeEnableCheatCode:
            neededParameters = 1;
            break;

        case GetRangeOfValue:
        case SetValue:
            neededParameters = 2;
            break;

        case Connect:
            neededParameters = 4;
            break;

        case SendCheatCode:
            neededParameters = (unsigned int)request.nbParameters;
            break;
    };

    assert(request.nbParameters == neededParameters);

    return true;
}
