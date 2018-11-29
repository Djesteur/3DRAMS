#ifndef REQUESTTRANSLATER_HPP
#define REQUESTTRANSLATER_HPP

#define MAX_MESSAGE_SIZE 65536

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

enum RequestType {

    Connect = 0,
    Disconnect = 1,
    GetValue = 2,
    GetRangeOfValue = 3,
    SetValue = 4,
    SendCheatCode = 5,
    DeleteCheatCode = 6,
    ChangeEnableCheatCode = 7,
    GetSetOfValue = 8
};

enum Module {

    Informations = 0,
    Connection = 1
};

typedef struct {

    enum Module requester;
    enum RequestType type;
    uint16_t nbParameters;
    uint8_t *parameters;

} Request;

uint16_t requestToString(const Request request, char *msg);
Request stringToRequest(const char *msg, const uint16_t msgSize); //Must be free !

void freeRequestParameters(Request request);

bool hasValidNumberOfParameters(const Request request);

#endif
