#include "ConnectionToPlugin.hpp"
#include "AbstractModule.hpp"

ConnectionToPlugin::ConnectionToPlugin():
    m_connectionSocket{-1},
    m_selectTimeout{0, 100'00},
    m_haveToStop{false},
    m_isConnected{false} {

    m_connectionSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_connectionSocket < 0) { throw std::runtime_error("ERROR: can't create a new socket."); }
}

ConnectionToPlugin::~ConnectionToPlugin() { close(m_connectionSocket); }

void ConnectionToPlugin::addModule(const Module moduleType, AbstractModule &module) { m_modules.emplace(moduleType, &module); }

void ConnectionToPlugin::run() {

    FD_ZERO(&m_waitingFD);
    FD_SET(m_connectionSocket, &m_waitingFD);

    while(!m_haveToStop){

        ::select(m_connectionSocket+1, &m_waitingFD, nullptr, nullptr, &m_selectTimeout);
        if(FD_ISSET(m_connectionSocket, &m_waitingFD)) {

            char message[MAX_REQUEST_SIZE];
            ::recv(m_connectionSocket, message, MAX_REQUEST_SIZE, 0);
            Request *result = stringToRequest(message);

            newResultReceived(*result);
            freeRequest(result);
        }

        FD_ZERO(&m_waitingFD);
        FD_SET(m_connectionSocket, &m_waitingFD);
    }
}

void ConnectionToPlugin::newRequest(const Request request) {

   if(request.type == RequestType::Connect) {

        std::string address;
        address += std::to_string(request.parameters[0]) + ".";
        address += std::to_string(request.parameters[1]) + ".";
        address += std::to_string(request.parameters[2]) + ".";
        address += std::to_string(request.parameters[3]);
        connectToServer(address, 10700);
    }

   else { sendRequest(request); }
}

void ConnectionToPlugin::stopProgram() { m_haveToStop = true; }

void ConnectionToPlugin::disconnect() {

   //Disconnect command received from server

    m_connectionSocket = -1;
    m_isConnected = false;
}

void ConnectionToPlugin::sendRequest(const Request &request) {

    char message[MAX_MESSAGE_SIZE];
    uint32_t messageSize{requestToString(&request, message)};
    ::send(m_connectionSocket, &message[0], messageSize, 0);
}

void ConnectionToPlugin::connectToServer(const std::string address, const short unsigned int port) {

    struct sockaddr_in addr;

    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;

    inet_aton(&address[0], &addr.sin_addr);
    int connectResult{0};
    // :: at the begin ensure that the compiler will not confuse with the QObject::connect function
    connectResult = ::connect(m_connectionSocket, (sockaddr*)(&addr), sizeof(struct sockaddr_in));
    m_isConnected = true;
    //A rendre non blocable

    Request *connectionResult{::newRequest(Module::Connection, RequestType::Connect)};

    emit m_modules[Module::Informations]->newResult(*connectionResult);
    emit m_modules[Module::Connection]->newResult(*connectionResult);

    freeRequest(connectionResult);
}

void ConnectionToPlugin::newResultReceived(const Request &request) {

    emit m_modules[request.requester]->newResult(request);
    emit m_modules[Module::Informations]->newResult(request);
    if(request.type == RequestType::Disconnect) { disconnect(); }
}
