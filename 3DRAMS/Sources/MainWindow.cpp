#include "ConnectionModule.hpp"
#include "InformationModule.hpp"
#include "SurveilModule.hpp"
#include "MainWindow.hpp"

MainWindow::MainWindow(std::list<Request*> &requestList, std::list<Request*> &resultList,
                       std::mutex &requestMutex, std::mutex &resultMutex, QWidget *parent):
    QMainWindow{parent},
    m_informationModule{this},
    m_connectionModule{this},
    m_surveilModule{this},
    m_requestList{requestList},
    m_resultList{resultList},
    m_requestMutex{requestMutex},
    m_resultMutex{resultMutex} {

    m_constantModules.addWidget(&m_informationModule);
    m_constantModules.addWidget(&m_connectionModule);

    m_mainLayout.addLayout(&m_constantModules);
    m_mainLayout.addWidget(&m_surveilModule);

    m_mainWidget.setLayout(&m_mainLayout);
    setCentralWidget(&m_mainWidget);

    connect(&m_timer, &QTimer::timeout, this, &MainWindow::update);
    m_timer.start(100);
}


void MainWindow::newRequest(Request *request) {

    m_requestMutex.lock();
    m_requestList.emplace_back(request);
    m_requestMutex.unlock();
}

void MainWindow::update() {

    if(!m_resultList.empty()) {

        m_resultMutex.lock();

        Request *lastResult;

        while(!m_resultList.empty()) {

            lastResult = m_resultList.front();

            emit m_informationModule.newResult(*lastResult);

            switch(lastResult->requester) {

                case Module::Connection:
                    emit m_connectionModule.newResult(*lastResult);
                    break;

                case Module::Surveil:
                    emit m_surveilModule.newResult(*lastResult);
                    break;

                default:
                    break;
            }

            m_resultList.pop_front();
            freeRequest(lastResult);
        }

        m_resultMutex.unlock();
    }
}
