#include "MainWindow.hpp"
#include "AbstractModule.hpp"

AbstractModule::AbstractModule(MainWindow *mainWindow, QWidget *parent):
    QWidget{parent},
    m_mainWindow{mainWindow} {}

AbstractModule::~AbstractModule() {}
