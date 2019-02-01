#ifndef ABSTRACTMODULE_HPP
#define ABSTRACTMODULE_HPP

#include <mutex>
#include <list>

#include <QWidget>

#include "Request.hpp"
#include "Converter.hpp"
#include "RequestTransmitter.hpp"

class AbstractModule : public QWidget {

    Q_OBJECT

    public:

        AbstractModule(RequestTransmitter &transmitter, QWidget *parent = nullptr);

        virtual ~AbstractModule();

    public slots:

        virtual void newResult(Request result) = 0;

    protected:

        RequestTransmitter &m_transmitter;
};

#endif
