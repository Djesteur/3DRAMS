#ifndef TCPCONNECTION_HPP
#define TCPCONNECTION_HPP

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <string>

class TcpConnection {

    public:

        TcpConnection();

        bool isValidConnection(std::string &reason) const;

        bool connectToServer(const std::string adress, const short unsigned int port);
        void disconnect();

        bool isWaitingDatas() const;

        std::string receive();
        void sendToServer(const std::string datasToSend);

    private:

        bool m_isValidConnection;
        std::string m_validReason;

        int m_connectionSocket;
        fd_set m_waitingFD;
};

#endif
