#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QTabWidget>

#include "ConnectionModule.hpp"
#include "InformationModule.hpp"

class MainWindow : public QMainWindow {

    Q_OBJECT

    public:

        MainWindow(ConnectionToPlugin &connectionToPlugin, QWidget *parent = nullptr);

    private:

        QWidget m_mainWidget;

        QHBoxLayout m_mainLayout;

            QVBoxLayout m_constantModules;

                InformationModule m_informationModule;
                ConnectionModule m_connectionModule;
};

#endif
