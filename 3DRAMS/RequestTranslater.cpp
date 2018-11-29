#include "RequestTranslater.hpp"

std::string requestToString(const Request &request) {

    std::string result;

    hasValidNumberOfParameters(request);

    result += static_cast<char>(request.requester);
    result += static_cast<char>(request.type);

    uint16_t splitedSize{static_cast<uint16_t>(request.parameters.size())};
    splitedSize >>= 8;
    result += static_cast<char>(splitedSize);
    result += static_cast<char>(request.parameters.size());

    for(uint8_t currentParam: request.parameters) { result += static_cast<char>(currentParam); }

    return result;
}

Request stringToRequest(const std::string &message) {

    Request finalRequest;

    if(message.size() < 4) { return finalRequest; }

    finalRequest.requester = static_cast<Module>(message[0]);
    finalRequest.type = static_cast<RequestType>(message[1]);

    uint16_t nbParams{static_cast<uint16_t>(message[2])};
    nbParams <<= 8;
    nbParams += static_cast<uint16_t>(message[3]);

    finalRequest.parameters.resize(nbParams);

    for(uint16_t i{4}; i < finalRequest.parameters.size() && i < message.size(); i++) { finalRequest.parameters[i] = static_cast<uint8_t>(message[i]);  }

    hasValidNumberOfParameters(finalRequest);

    return finalRequest;
}


bool hasValidNumberOfParameters(const Request &request) {

    unsigned int neededParameters = 0;
    switch(request.type) {

        case Connect:
            neededParameters = 0;
            break;

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

        case SendCheatCode:
        case GetSetOfValue:
            neededParameters = static_cast<unsigned int>(request.parameters.size());
            break;
    };

    if(neededParameters != request.parameters.size()) { throw std::runtime_error("Error invalid parameters size (" + std::to_string(neededParameters) + " expected, received "
                                                                                  + std::to_string(request.parameters.size()) + ")."); }

    return true;
}
