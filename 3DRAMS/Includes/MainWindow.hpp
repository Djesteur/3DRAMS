#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <mutex>

#include <QMainWindow>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>

#include <QTimer>

#include "Request.hpp"

class ConnectionModule;
class InformationModule;
class SurveilModule;

class MainWindow : public QMainWindow {

    Q_OBJECT

    public:

        MainWindow(std::list<Request*> &requestList, std::list<Request*> &resultList,
                   std::mutex &requestMutex, std::mutex &resultMutex, QWidget *parent = nullptr);

    public slots:

        virtual void newRequest(Request *request);
        virtual void update();

    private:

        QWidget m_mainWidget;

        QHBoxLayout m_mainLayout;

            QVBoxLayout m_constantModules;

                InformationModule *m_informationModule;
                ConnectionModule *m_connectionModule;

            SurveilModule *m_surveilModule;

        QTimer m_timer;

        std::list<Request*> &m_requestList, &m_resultList;
        std::mutex &m_requestMutex, &m_resultMutex;
};

#endif
