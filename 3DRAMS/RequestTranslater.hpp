#ifndef REQUESTTRANSLATER_HPP
#define REQUESTTRANSLATER_HPP

#include <string>
#include <vector>
#include <stdexcept>

#define MAX_PARAMETERS_SIZE 65532
// 2^16 - 4 bytes for others informations
#define MAX_MESSAGE_SIZE 65536

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
    std::vector<uint8_t> parameters;

} Request;

std::string requestToString(const Request &request);
Request stringToRequest(const std::string &message);
bool hasValidNumberOfParameters(const Request &request);

#endif
