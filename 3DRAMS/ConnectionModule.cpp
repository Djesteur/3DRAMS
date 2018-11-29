#include "ConnectionModule.hpp"

ConnectionModule::ConnectionModule(ConnectionToPlugin &connectionToPlugin, QWidget *parent):
    AbstractModule{connectionToPlugin, parent},
    m_maxIPChar{15} {

    m_connectionButton.setText(tr("Connexion"));
    m_disconnectionButton.setText(tr("Déconnexion"));
    m_disconnectionButton.setEnabled(false);

    m_connectionIP.setPlainText("192.168.1.10");
    m_connectionIP.setFixedHeight(25);
    m_connectionIP.setFixedWidth(101);

    m_mainLayout.addWidget(&m_connectionIP);
    m_mainLayout.addWidget(&m_connectionButton);
    m_mainLayout.addWidget(&m_disconnectionButton);

    setLayout(&m_mainLayout);

    connect(&m_connectionIP, &QPlainTextEdit::textChanged, this, &ConnectionModule::limitIPSize);
    connect(&m_connectionButton, &QPushButton::clicked, this, &ConnectionModule::connectButtonSlot);
    connect(&m_disconnectionButton, &QPushButton::clicked, this, &ConnectionModule::disconnectButtonSlot);
}


ConnectionModule::~ConnectionModule() {}



void ConnectionModule::newResult(Request result) {

    switch(result.type) {

        case RequestType::Connect:
            if(result.parameters[0] == 0x00000000) {

                m_connectionButton.setEnabled(false);
                m_disconnectionButton.setEnabled(true);
            }
            break;

        case RequestType::Disconnect:
            m_connectionButton.setEnabled(true);
            m_disconnectionButton.setEnabled(false);
            break;

        default:
            break;
    }
}

void ConnectionModule::connectButtonSlot() {

    Request request;
    request.type = RequestType::Connect;
    request.requester = Module::Connection;
    request.parameters.resize(4);

    std::vector<std::string> splitedString{splitDatas(m_connectionIP.toPlainText().toUtf8().constData(), '.')};

    if(splitedString.size() == 4) {

        request.parameters[0] = static_cast<uint8_t>(std::stoul(splitedString[0]));
        request.parameters[1] = static_cast<uint8_t>(std::stoul(splitedString[1]));
        request.parameters[2] = static_cast<uint8_t>(std::stoul(splitedString[2]));
        request.parameters[3] = static_cast<uint8_t>(std::stoul(splitedString[3]));

        emit m_connectionToPlugin.newRequest(request);
    }
}

void ConnectionModule::disconnectButtonSlot() {

    Request request;
    request.type = RequestType::Disconnect;
    request.requester = Module::Connection;
    request.parameters = {0};

    emit m_connectionToPlugin.newRequest(request);
}


void ConnectionModule::limitIPSize() {

    if(m_connectionIP.toPlainText().length() > m_maxIPChar) { m_connectionIP.textCursor().deletePreviousChar();  }
}



std::vector<std::string> splitDatas(const std::string &datas, const char spliter) {

    std::stringstream ss{datas};
    std::string currentData;
    std::vector<std::string> splited;

    while (std::getline(ss, currentData, spliter)) { splited.push_back(currentData); }

    return splited;
}

