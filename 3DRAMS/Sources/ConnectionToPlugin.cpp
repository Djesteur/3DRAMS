#include "ConnectionToPlugin.hpp"

ConnectionToPlugin::ConnectionToPlugin(RequestTransmitter &transmitter):
    m_transmitter{transmitter},
    m_connectionSocket{socket(AF_INET, SOCK_STREAM, 0)},
    m_selectTimeout{0, 100'00},
    m_haveToStop{false},
    m_isConnected{false},
    m_alreadyBeenConnected{false} {

    if(m_connectionSocket < 0) { throw std::runtime_error("ERROR: can't create a new socket."); }
}

ConnectionToPlugin::~ConnectionToPlugin() { close(m_connectionSocket); }

void ConnectionToPlugin::run() {

    FD_ZERO(&m_waitingFD);
    FD_SET(m_connectionSocket, &m_waitingFD);

    while(!m_haveToStop){

        select(m_connectionSocket+1, &m_waitingFD, nullptr, nullptr, &m_selectTimeout);

        if(m_isConnected && FD_ISSET(m_connectionSocket, &m_waitingFD)) {

            char message[MAX_REQUEST_SIZE];
            int recvResult = recv(m_connectionSocket, message, MAX_REQUEST_SIZE, 0);
            if(recvResult == 0) { //Server disconnected

                Request *result = newRequest(Module::Informations, RequestType::Disconnect);
                m_transmitter.addResult(result);
                disconnect();
            }

            else {

                Request *result = stringToRequest(message);
                std::cout << "Recu" << std::endl;
                m_transmitter.addResult(result);


                if(result->type == RequestType::Disconnect) { disconnect(); }
            }
        }

        FD_ZERO(&m_waitingFD);
        FD_SET(m_connectionSocket, &m_waitingFD);

        Request *lastRequest;

        while(m_transmitter.waitingRequest()) {

            lastRequest = m_transmitter.getLastRequest();

            if(lastRequest->type == RequestType::Connect) {

                std::string address;
                address += std::to_string(lastRequest->parameters[0]) + ".";
                address += std::to_string(lastRequest->parameters[1]) + ".";
                address += std::to_string(lastRequest->parameters[2]) + ".";
                address += std::to_string(lastRequest->parameters[3]);
                connectToServer(address, 10700);
            }

           else { sendRequest(*lastRequest); }

           freeRequest(lastRequest);
        }
    }
}

void ConnectionToPlugin::disconnect() { m_isConnected = false; }

void ConnectionToPlugin::stopProgram() { m_haveToStop = true; }

void ConnectionToPlugin::sendRequest(const Request &request) {

    char message[MAX_REQUEST_SIZE];
    uint32_t messageSize{requestToString(&request, message)};
    send(m_connectionSocket, &message[0], messageSize, 0);

    //sleep(1);// Wait 1 millisecond, need it for server. Else, he will fuse some packet and create problems
                 //Different function on Windows ?
}

void ConnectionToPlugin::connectToServer(const std::string address, const short unsigned int port) {

    int connectResult{0};

    if(!m_alreadyBeenConnected) {

        struct sockaddr_in addr;

        addr.sin_port = htons(port);
        addr.sin_family = AF_INET;

        inet_aton(&address[0], &addr.sin_addr);

        connectResult = connect(m_connectionSocket, (sockaddr*)(&addr), sizeof(struct sockaddr_in));
    }

    if(connectResult < 0) { 

        connectResult = 1;
        m_isConnected = false; 
    }

    else { m_isConnected = true; m_alreadyBeenConnected = true; }

    Request *connectionResult{newRequest(Module::Connection, RequestType::Connect)};
    writeByte(connectionResult, static_cast<uint8_t>(connectResult));

    m_transmitter.addResult(connectionResult);
}