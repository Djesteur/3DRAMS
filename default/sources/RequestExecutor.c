#include "RequestExecutor.h"

ResultArray* initResultArray(const Request request) {

    ResultArray *newArray = (ResultArray*) malloc(sizeof(ResultArray));

    newArray->nbRequest = 0;
    newArray->currentParametersIndex = 0;
    newArray->requestArray = (Request*) malloc(sizeof(Request)*MAX_REQUEST_RESULT);

    for(uint16_t i = 0; i < MAX_REQUEST_RESULT; i++) {

        newArray->requestArray[i].type = request.type;
        newArray->requestArray[i].requester = request.requester;
        newArray->requestArray[i].nbParameters = 0;
        newArray->requestArray[i].parameters = (uint8_t*) malloc(MAX_MESSAGE_SIZE*sizeof(uint8_t));
    }

    return newArray;
}

void addValue(ResultArray *resultArray, const uint8_t newValue) {

    if(resultArray->currentParametersIndex + 1 > MAX_MESSAGE_SIZE) {

        if(resultArray->nbRequest + 1 > MAX_REQUEST_RESULT) { return; } //Totally full, can't add more

        resultArray->nbRequest += 1;
        resultArray->currentParametersIndex = 0;

        resultArray->requestArray[resultArray->nbRequest].parameters[resultArray->currentParametersIndex] = newValue;
        resultArray->requestArray[resultArray->nbRequest].nbParameters += 1;
    }

    else {

        resultArray->requestArray[resultArray->nbRequest].parameters[resultArray->currentParametersIndex] = newValue;
        resultArray->requestArray[resultArray->nbRequest].nbParameters += 1;
        resultArray->currentParametersIndex += 1;
    }
}

void addPairOfValue(ResultArray *resultArray, const uint8_t firstValue, const uint8_t secondValue) {

    if(resultArray->currentParametersIndex + 2 >= MAX_MESSAGE_SIZE) {

        if(resultArray->nbRequest + 1 > MAX_REQUEST_RESULT) { return; } //Totally full, can't add more

        resultArray->nbRequest += 1;
        resultArray->currentParametersIndex = 0;

        resultArray->requestArray[resultArray->nbRequest].parameters[resultArray->currentParametersIndex] = firstValue;
        resultArray->requestArray[resultArray->nbRequest].parameters[resultArray->currentParametersIndex+1] = secondValue;
        resultArray->requestArray[resultArray->nbRequest].nbParameters += 2;
    }

    else { 

        resultArray->requestArray[resultArray->nbRequest].parameters[resultArray->currentParametersIndex] = firstValue;
        resultArray->requestArray[resultArray->nbRequest].parameters[resultArray->currentParametersIndex+1] = secondValue;
        resultArray->requestArray[resultArray->nbRequest].nbParameters += 2;
        resultArray->currentParametersIndex += 2; 
    }
}

void freeResultArray(ResultArray *resultArray) {

    if(resultArray != NULL) {

        uint16_t i = 0;

        bool haveToStop = false;

        while(!haveToStop) {

            if(resultArray->requestArray[i].parameters == NULL) { haveToStop = true; }
            else { free(resultArray->requestArray[i].parameters); }

            i++;
        }

        free(resultArray->requestArray);
        free(resultArray);
    }
}


ResultArray* executeRequest(const char *msg, const uint16_t msgSize) {

    ResultArray *resultArray;
    Request newRequest = stringToRequest(msg, msgSize);
    resultArray = initResultArray(newRequest);

    switch(newRequest.type) {

       case Connect:
            connectRequest(resultArray);
            break;

        case Disconnect:
        	disconnectRequest(resultArray);
            break;

        case GetValue:
        	getValueRequest(resultArray);
            break;
                
        case GetRangeOfValue:
        	getRangeOfValueRequest(resultArray);
            break;

        case SetValue:
        	setValueRequest(resultArray);
            break;

        case SendCheatCode:
        	sendCheatCodeRequest(resultArray);
            break;

        case DeleteCheatCode:
        	deleteCheatCodeRequest(resultArray);
            break;

        case ChangeEnableCheatCode:
        	changeEnableCheatCodeRequest(resultArray);
            break;

        case GetSetOfValue:
        	getSetOfValueRequest(resultArray);
            break;
    }

    freeRequestParameters(newRequest);

    return resultArray;
}

void connectRequest(ResultArray *resultArray) {}
void disconnectRequest(ResultArray *resultArray) { addValue(resultArray, 0x00000000); }

void getValueRequest(ResultArray *resultArray) {}
void getRangeOfValueRequest(ResultArray *resultArray) {}
void setValueRequest(ResultArray *resultArray) {}
void sendCheatCodeRequest(ResultArray *resultArray) {}
void deleteCheatCodeRequest(ResultArray *resultArray) {}
void changeEnableCheatCodeRequest(ResultArray *resultArray) {}
void getSetOfValueRequest(ResultArray *resultArray) {}