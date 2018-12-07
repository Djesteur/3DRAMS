#ifndef REQUESTEXECUTOR_H
#define REQUESTEXECUTOR_H

#include "Request.h"
#include "Converter.h"

//Return the result (as a request)

Request* executeRequest(const char *msg);

Request* connectRequest(Request *request);
Request* disconnectRequest(Request *request);
Request* getValueRequest(Request *request);
Request* getRangeOfValueRequest(Request *request);
Request* setValueRequest(Request *request);
Request* sendCheatCodeRequest(Request *request);
Request* deleteCheatCodeRequest(Request *request);
Request* changeEnableCheatCodeRequest(Request *request);
Request* getSetOfValueRequest(Request *request);

#endif
