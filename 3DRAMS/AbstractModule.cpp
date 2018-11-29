#include "AbstractModule.hpp"

AbstractModule::AbstractModule(ConnectionToPlugin &connectionToPlugin, QWidget *parent):
    QWidget{parent},
    m_connectionToPlugin{connectionToPlugin} {}

AbstractModule::~AbstractModule() {}
