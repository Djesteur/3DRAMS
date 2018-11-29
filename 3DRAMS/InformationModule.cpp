#include "InformationModule.hpp"

InformationModule::InformationModule(ConnectionToPlugin &connectionToPlugin, QWidget *parent):
    AbstractModule{connectionToPlugin, parent} {

    m_text.setReadOnly(true);
    m_text.setFixedHeight(600);
    m_text.setFixedWidth(300);

    m_mainLayout.addWidget(&m_text);
    setLayout(&m_mainLayout);
}

InformationModule::~InformationModule() {}

void InformationModule::newResult(Request result) {

    switch(result.requester) {

        case Module::Connection:
            connectionInfo(result);
            break;

        default:

            break;
    }
}

void InformationModule::insertInfos(QString info) {

    m_text.moveCursor(QTextCursor::End);
    m_text.insertPlainText(info);
    m_text.moveCursor(QTextCursor::End);
}


void InformationModule::connectionInfo(Request result) {

    switch(result.type) {

        case RequestType::Connect:
            if(result.parameters[0] == 0x00000000) { insertInfos(tr("Connecté !\n")); }
            else { insertInfos(tr("Impossible de se connecter.\n")); }
            break;

        case RequestType::Disconnect:
            insertInfos(tr("Déconnecté.\n"));
            break;

        default:
            break;
    }
}
