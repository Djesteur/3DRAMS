#include "Request.hpp"

uint32_t requestToString(const Request *request, char *msg) {

    msg[0] = (char)(request->requester);
    msg[1] = (char)(request->type);

    uint16_t splitedSize = (uint16_t)(request->nbParameters);
    splitedSize >>= 8;
    msg[2] = (char)(splitedSize);
    msg[3] = (char)(request->nbParameters);

    for(uint16_t i = 0; i < request->nbParameters; i++) { msg[4+i] = (char)(request->parameters[i]); }

    return 4 + request->nbParameters;
}

Request* stringToRequest(const char *msg) {

    Request *finalRequest = newRequest((enum Module)msg[0], (enum RequestType)msg[1]);

    finalRequest->nbParameters = (uint16_t)msg[2];
    finalRequest->nbParameters <<= 8;
    finalRequest->nbParameters += (uint16_t)msg[3];

    for(uint16_t i = 0; i < finalRequest->nbParameters; i++) { finalRequest->parameters[i] = (uint8_t)msg[4+i];  }

    return finalRequest;
}

void writeByte(Request *request, const uint8_t byte) {

    if(request->nbParameters + 1 < MAX_MESSAGE_SIZE) {

        request->parameters[request->nbParameters] = byte;
        ++request->nbParameters;
    }
}

void writeNumber(Request *request, const uint32_t number) {

    if(request->nbParameters + 4 < MAX_MESSAGE_SIZE) {

        uint32_t numberToInsert = 0;

        for(uint8_t i = 0; i < 4; i++) {

            numberToInsert = number;
            numberToInsert <<= 8*i;
            numberToInsert >>= 24;
            request->parameters[request->nbParameters+i] = (uint8_t)(numberToInsert);
        }

        request->nbParameters += 4;
    }
}


Request* newRequest(const enum Module requester, const enum RequestType type) {

    Request *newRequest = (Request*) malloc(sizeof(Request));
    newRequest->requester = requester;
    newRequest->type = type;
    newRequest->nbParameters = 0;
    newRequest->parameters = (uint8_t*) malloc(sizeof(uint8_t)*MAX_MESSAGE_SIZE);
    return newRequest;
}

void freeRequest(Request *request) {

    if(request->parameters != NULL) { 

        free(request->parameters); 
        request->parameters = NULL;
    }
}