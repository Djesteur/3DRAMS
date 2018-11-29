#ifndef REQUESTEXECUTOR_HPP
#define REQUESTEXECUTOR_HPP

#include "RequestTranslater.h"

#define MAX_REQUEST_RESULT 3

typedef struct {

	Request *requestArray;
	uint16_t nbRequest;
	uint16_t currentParametersIndex;

} ResultArray;


ResultArray* initResultArray(const Request request);

void addValue(ResultArray *resultArray, const uint8_t newValue);
void addPairOfValue(ResultArray *resultArray, const uint8_t firstValue, const uint8_t secondValue);

void freeResultArray(ResultArray *resultArray);

//Return the result (as a request)

ResultArray* executeRequest(const char *msg, const uint16_t msgSize);

void connectRequest(ResultArray *resultArray);
void disconnectRequest(ResultArray *resultArray);
void getValueRequest(ResultArray *resultArray);
void getRangeOfValueRequest(ResultArray *resultArray);
void setValueRequest(ResultArray *resultArray);
void sendCheatCodeRequest(ResultArray *resultArray);
void deleteCheatCodeRequest(ResultArray *resultArray);
void changeEnableCheatCodeRequest(ResultArray *resultArray);
void getSetOfValueRequest(ResultArray *resultArray);

#endif
