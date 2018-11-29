#ifndef ABSTRACTMODULE_HPP
#define ABSTRACTMODULE_HPP

#include <mutex>
#include <list>

#include <QWidget>

#include "ConnectionToPlugin.hpp"


class AbstractModule : public QWidget {

    Q_OBJECT

    public:

        AbstractModule(ConnectionToPlugin &connectionToPlugin, QWidget *parent = nullptr);

        virtual ~AbstractModule();

    public slots:

        virtual void newResult(Request result) = 0;

    protected:

        ConnectionToPlugin &m_connectionToPlugin;
};

#endif
