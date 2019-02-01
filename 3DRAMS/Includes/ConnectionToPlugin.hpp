#ifndef CONNECTIONTOPLUGIN_HPP
#define CONNECTIONTOPLUGIN_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <iostream>
#include <string>
#include <array>
#include <map>

#include <string.h>

#include "Request.hpp"
#include "RequestTransmitter.hpp"

class ConnectionToPlugin {

    public:

        ConnectionToPlugin(RequestTransmitter &transmitter);
        ~ConnectionToPlugin();

        ConnectionToPlugin(const ConnectionToPlugin&) = delete;
        ConnectionToPlugin &operator=(const ConnectionToPlugin&) = delete;

        void run();

        void stopProgram();

    private:

        void connectToServer(const std::string adress, const short unsigned int port);
        void disconnect();
        void sendRequest(const Request &request);

        RequestTransmitter &m_transmitter;

        const int m_connectionSocket;
        fd_set m_waitingFD;
        struct timeval m_selectTimeout;

        bool m_haveToStop, m_isConnected, m_alreadyBeenConnected;
};

#endif
