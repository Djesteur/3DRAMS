#ifndef CONNECTIONMODULE_HPP
#define CONNECTIONMODULE_HPP

#include <sstream>
#include <iostream>

#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>

#include "AbstractModule.hpp"

class ConnectionModule : public AbstractModule {

    Q_OBJECT

    public:

        ConnectionModule(RequestTransmitter &transmitter, QWidget *parent = nullptr);

        virtual ~ConnectionModule();

    public slots:

        virtual void newResult(Request result);

        void connectButtonSlot();
        void disconnectButtonSlot();

    private:

        QHBoxLayout m_mainLayout;
            QLineEdit m_connectionIP;
            QPushButton m_connectionButton;
            QPushButton m_disconnectionButton;

        const unsigned int m_maxIPChar;
};

std::vector<std::string> splitDatas(const std::string &datas, const char spliter);

#endif
