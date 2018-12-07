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

#include <QObject>

#include "Request.hpp"

class AbstractModule;

class ConnectionToPlugin: public QObject {

    Q_OBJECT

    public:

        ConnectionToPlugin();
        ~ConnectionToPlugin();

        ConnectionToPlugin(const ConnectionToPlugin&) = delete;
        ConnectionToPlugin &operator=(const ConnectionToPlugin&) = delete;

        void addModule(const Module moduleType, AbstractModule &module);
        void run();

    public slots:

        void newRequest(const Request request);
        void stopProgram();

    private:

        void connectToServer(const std::string adress, const short unsigned int port);
        void disconnect();
        void sendRequest(const Request &request);
        void newResultReceived(const Request &request);

        int m_connectionSocket;
        fd_set m_waitingFD;
        struct timeval m_selectTimeout;

        bool m_haveToStop, m_isConnected;

        std::map<Module, AbstractModule*> m_modules;
};

#endif
