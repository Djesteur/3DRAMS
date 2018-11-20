#include "TcpConnection.hpp"

TcpConnection::TcpConnection():
    m_isValidConnection{true},
    m_connectionSocket{-1} {

    m_connectionSocket = socket(AF_INET, SOCK_STREAM, 0);
    if(m_connectionSocket < 0) {

        m_isValidConnection = false;
        m_validReason = "Can't create socket.";
    }

    FD_ZERO(&m_waitingFD);
    FD_SET(m_connectionSocket, &m_waitingFD);
}

bool TcpConnection::isValidConnection(std::string &reason) const {

    reason = m_validReason;
    return m_isValidConnection;
}

bool TcpConnection::connectToServer(const std::string adress, const short unsigned int port) {

    struct sockaddr_in addr;

    addr.sin_port = htons(port);
    addr.sin_family = AF_INET;

    if(inet_aton(&adress[0], &addr.sin_addr) == 0) { return false; }

    if(connect(m_connectionSocket, (sockaddr*)(&addr), sizeof(struct sockaddr_in)) < 0) { return false; }

    return true;
}

void TcpConnection::disconnect() {

}

bool TcpConnection::isWaitingDatas() const {

    return false;
}

std::string TcpConnection::receive() {

       return "";
}

void TcpConnection::sendToServer(const std::string datasToSend) {

    send(m_connectionSocket, &datasToSend[0], 32, 0);
}
