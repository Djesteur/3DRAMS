#ifndef REQUESTEXECUTOR_H
#define REQUESTEXECUTOR_H

#include <3ds.h>

#include "csvc.h"

#include "Request.h"
#include "Converter.h"

#define THREADVARS_MAGIC  0x21545624 // !TV$

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
Request* freezeGameRequest(Request *request);

//By Nanquitas

static bool ThreadPredicate(u32 *kthread);
void FreezeGameFunction();
void UnfreezeGameFunction();

#endif
