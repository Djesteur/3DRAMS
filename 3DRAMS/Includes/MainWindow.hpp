#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <mutex>

#include <QMainWindow>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTabWidget>

#include <QTimer>

#include "Request.hpp"
#include "RequestTransmitter.hpp"

#include "ConnectionModule.hpp"
#include "InformationModule.hpp"
#include "SurveilModule.hpp"
#include "SeekerModule.hpp"

class MainWindow : public QMainWindow {

    Q_OBJECT

    public:

        MainWindow(RequestTransmitter &transmitter, QWidget *parent = nullptr);

    public slots:

        virtual void update();

    private:

        RequestTransmitter &m_transmitter;

        QWidget m_mainWidget;

        QHBoxLayout m_mainLayout;

            QVBoxLayout m_constantModules;

                InformationModule m_informationModule;
                ConnectionModule m_connectionModule;

            QTabWidget m_variableModules;

                SurveilModule m_surveilModule;
                SeekerModule m_seekerModule;

        QTimer m_timer;
};

#endif
