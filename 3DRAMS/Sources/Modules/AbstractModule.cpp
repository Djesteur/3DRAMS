#include "AbstractModule.hpp"

AbstractModule::AbstractModule(RequestTransmitter &transmitter, QWidget *parent):
    QWidget{parent},
    m_transmitter{transmitter} {}

AbstractModule::~AbstractModule() {}
