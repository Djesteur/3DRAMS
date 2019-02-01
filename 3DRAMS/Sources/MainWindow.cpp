#include "MainWindow.hpp"

MainWindow::MainWindow(RequestTransmitter &transmitter, QWidget *parent):
    QMainWindow{parent},
    m_transmitter{transmitter},
    m_informationModule{transmitter},
    m_connectionModule{transmitter},
    m_surveilModule{transmitter},
    m_seekerModule{transmitter} {

    //Constant
    m_constantModules.addWidget(&m_informationModule);
    m_constantModules.addWidget(&m_connectionModule);

    m_constantModules.setSizeConstraint(QLayout::SetFixedSize);

    m_mainLayout.addLayout(&m_constantModules);

    //Variable
    m_variableModules.addTab(&m_seekerModule, "Seeker");
    m_variableModules.addTab(&m_surveilModule, "Surveil");

    m_mainLayout.addWidget(&m_variableModules);

    m_mainWidget.setLayout(&m_mainLayout);
    setCentralWidget(&m_mainWidget);

    connect(&m_timer, &QTimer::timeout, this, &MainWindow::update);
    m_timer.start(500);
}

void MainWindow::update() {

    Request *lastResult;

    while(m_transmitter.waitingResult()) {

        lastResult = m_transmitter.getLastResult();

        emit m_informationModule.newResult(*lastResult);

        switch(lastResult->requester) {

            case Module::Connection:
                emit m_connectionModule.newResult(*lastResult);
                break;

            case Module::Surveil:
                emit m_surveilModule.newResult(*lastResult);
                break;

            case Module::Seeker:
                emit m_seekerModule.newResult(*lastResult);

            default:
                break;
        }

        freeRequest(lastResult);
    }
}
