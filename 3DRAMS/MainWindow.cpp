#include "MainWindow.hpp"

MainWindow::MainWindow(ConnectionToPlugin &connectionToPlugin, QWidget *parent):
    QMainWindow{parent},
    m_informationModule{connectionToPlugin},
    m_connectionModule{connectionToPlugin} {

    connectionToPlugin.addModule(Module::Informations, m_informationModule);
    connectionToPlugin.addModule(Module::Connection, m_connectionModule);

    m_constantModules.addWidget(&m_informationModule);
    m_constantModules.addWidget(&m_connectionModule);

    m_mainLayout.addLayout(&m_constantModules);

    m_mainWidget.setLayout(&m_mainLayout);
    setCentralWidget(&m_mainWidget);
}
