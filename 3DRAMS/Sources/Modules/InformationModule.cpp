#include "InformationModule.hpp"

InformationModule::InformationModule(RequestTransmitter &transmitter, QWidget *parent):
    AbstractModule{transmitter, parent} {

    m_text.setReadOnly(true);
    m_text.setFixedHeight(600);
    m_text.setFixedWidth(400);

    m_mainLayout.addWidget(&m_text);
    setLayout(&m_mainLayout);
}

InformationModule::~InformationModule() {}

void InformationModule::newResult(Request result) {

    switch(result.requester) {

        case Module::Informations:
            generalInfo(result);
            break;

        case Module::Connection:
            connectionInfo(result);
            break;

        default:

            break;
    }
}

void InformationModule::insertInfos(QString info, QColor textColor) {

    QString currentHour{QString::number(QTime::currentTime().hour())}, 
            currentMinute{QString::number(QTime::currentTime().minute())},
            currentSecond{QString::number(QTime::currentTime().second())};

    if(currentHour.count() < 2) { currentHour.push_front('0'); }
    if(currentMinute.count() < 2) { currentMinute.push_front('0'); }
    if(currentSecond.count() < 2) { currentSecond.push_front('0'); }

    QString finalInfo;

    finalInfo += "[";
    finalInfo += currentHour;
    finalInfo += ':';
    finalInfo += currentMinute;
    finalInfo += ':';
    finalInfo += currentSecond;
    finalInfo += "] ";
    finalInfo += info;
    finalInfo += "\n";

    m_text.setTextColor(textColor);

    m_text.moveCursor(QTextCursor::End);
    m_text.insertPlainText(finalInfo);
    m_text.moveCursor(QTextCursor::End);
}


void InformationModule::connectionInfo(Request result) {

    switch(result.type) {

        case RequestType::Connect:
            if(result.parameters[0] == 0x00000000) { insertInfos(tr("Connecté !"), QColor{0, 0, 255}); }
            else { insertInfos(tr("Impossible de se connecter."), QColor{255, 0, 0}); }
            break;

        case RequestType::Disconnect:
            insertInfos(tr("Déconnecté."), QColor{255, 0, 255});
            break;

        default:
            break;
    }
}


void InformationModule::generalInfo(Request result) {

    switch(result.type) {

        case RequestType::ResearchProgress: {

            uint32_t progressToConvert{parametersToNumber(&(result.parameters[0]))};
            float *progress{reinterpret_cast<float*>(&progressToConvert)};
            insertInfos(tr("Lecture des valeurs en cours: ") + QString::number(*progress) + "%.", QColor{0, 255, 255});
        }
            break; 

        default:
            break;
    }
}
