#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent):
    QMainWindow{parent},
    m_isConnected{false} {

    //Specific informations
    m_connectionInfos.setReadOnly(true);
    m_connectionInfos.setFixedHeight(400);
    m_connectionInfos.setFixedWidth(600);

    m_connectionButton.setText(tr("Connexion"));
    m_disconnectionButton.setText(tr("Déconnexion"));
    m_disconnectionButton.setEnabled(false);

    m_connectionIP.setPlainText("192.168.1.10");
    m_connectionIP.setFixedHeight(25);
    m_connectionIP.setFixedWidth(101);

    //Constructing visual position

    m_connectionInteraction.addWidget(&m_connectionIP);
    m_connectionInteraction.addWidget(&m_connectionButton);
    m_connectionInteraction.addWidget(&m_disconnectionButton);

    m_connectionLayout.addWidget(&m_connectionInfos);
    m_connectionLayout.addLayout(&m_connectionInteraction);

    m_connectionLayout.setStretchFactor(&m_connectionInfos, 4);
    m_connectionLayout.setStretchFactor(&m_connectionInteraction, 1);

    m_mainLayout.addLayout(&m_connectionLayout);
    m_mainLayout.addWidget(&m_tabs);

    m_mainLayout.setStretchFactor(&m_connectionLayout, 1);
    m_mainLayout.setStretchFactor(&m_tabs, 2);


    m_mainWidget.setLayout(&m_mainLayout);
    setCentralWidget(&m_mainWidget);



    //Signal and slots

    connect(&m_connectionButton, &QPushButton::released, this, &MainWindow::tryToConnect);


    connect(&m_connectionIP, &QPlainTextEdit::textChanged, this, &MainWindow::limitIPSize);
}

void MainWindow::tryToConnect() {

    m_connectionButton.setEnabled(false);
    if(m_isConnected) { m_isConnected = false; }

    std::string IP{m_connectionIP.toPlainText().toUtf8().constData()};
    QString message;

    const quint16 port{10700};

    message = tr("Tentative de connexion... ");
    insertInfos(message);

    if(m_connection.connectToServer(IP, port)) {

        insertInfos(tr("succès !\n"));
        m_isConnected = true;
        m_disconnectionButton.setEnabled(true);

        m_connection.sendToServer("oooooooooooooooooooooooooooooooo");
    }

    else { insertInfos(tr("échec.\n")); }

    if(!m_isConnected) {

        insertInfos(tr("Impossible de se connecter.\n"));
        m_connectionButton.setEnabled(true);
    }
}

void MainWindow::insertInfos(QString info) {

    m_connectionInfos.moveCursor(QTextCursor::End);
    m_connectionInfos.insertPlainText(info);
    m_connectionInfos.moveCursor(QTextCursor::End);
}

void MainWindow::limitIPSize() {

    int maxChar{15};
    if(m_connectionIP.toPlainText().length() > maxChar) { m_connectionIP.textCursor().deletePreviousChar();  }
}
