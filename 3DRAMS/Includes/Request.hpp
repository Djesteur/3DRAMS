#ifndef REQUEST_H
#define REQUEST_H

#define MAX_REQUEST_SIZE 65536
#define MAX_MESSAGE_SIZE 65532// 2^16 - 4

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
    Connection = 1,
    Surveil = 2
};

typedef struct {

    enum Module requester;
    enum RequestType type;
    uint16_t nbParameters;
    uint8_t *parameters;

} Request;

uint32_t requestToString(const Request *request, char *msg);
Request* stringToRequest(const char *msg);

void writeByte(Request *request, const uint8_t byte);
void writeNumber(Request *request, const uint32_t number);

Request* newRequest(const enum Module requester, const enum RequestType type);
void freeRequest(Request *request);

#endif
