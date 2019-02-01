#include "ConnectionModule.hpp"

ConnectionModule::ConnectionModule(RequestTransmitter &transmitter, QWidget *parent):
    AbstractModule{transmitter, parent},
    m_maxIPChar{15} {

    m_connectionButton.setText(tr("Connect"));
    m_disconnectionButton.setText(tr("Disconnect"));
    m_disconnectionButton.setEnabled(false);

    m_connectionIP.setInputMask("000.000.000.000");
    m_connectionIP.setText("192.168.001.010");
    m_connectionIP.setFixedHeight(25);
    m_connectionIP.setFixedWidth(101);

    m_mainLayout.addWidget(&m_connectionIP);
    m_mainLayout.addWidget(&m_connectionButton);
    m_mainLayout.addWidget(&m_disconnectionButton);

    setLayout(&m_mainLayout);

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
    
    std::vector<std::string> splitedString{splitDatas(m_connectionIP.text().toUtf8().constData(), '.')};

    if(splitedString.size() == 4) {

        Request *request{newRequest(Module::Connection, RequestType::Connect)};

        writeByte(request, static_cast<uint8_t>(std::stoul(splitedString[0])));
        writeByte(request, static_cast<uint8_t>(std::stoul(splitedString[1])));
        writeByte(request, static_cast<uint8_t>(std::stoul(splitedString[2])));
        writeByte(request, static_cast<uint8_t>(std::stoul(splitedString[3])));

        m_transmitter.addRequest(request);
    }
}

void ConnectionModule::disconnectButtonSlot() {

    Request *request{newRequest(Module::Connection, RequestType::Disconnect)};
    writeByte(request, 0);

    m_transmitter.addRequest(request);
}


std::vector<std::string> splitDatas(const std::string &datas, const char spliter) {

    std::stringstream ss{datas};
    std::string currentData;
    std::vector<std::string> splited;

    while (std::getline(ss, currentData, spliter)) { splited.push_back(currentData); }

    return splited;
}

