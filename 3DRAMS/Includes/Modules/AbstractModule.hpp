#ifndef ABSTRACTMODULE_HPP
#define ABSTRACTMODULE_HPP

#include <mutex>
#include <list>

#include <QWidget>

#include "Request.hpp"

class MainWindow;

class AbstractModule : public QWidget {

    Q_OBJECT

    public:

        AbstractModule(MainWindow *mainWindow, QWidget *parent = nullptr);

        virtual ~AbstractModule();

    public slots:

        virtual void newResult(Request result) = 0;

    protected:

        MainWindow *m_mainWindow;
};

#endif
