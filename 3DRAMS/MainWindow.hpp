#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QTabWidget>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QTextEdit>

#include "TcpConnection.hpp"

class MainWindow : public QMainWindow {

    Q_OBJECT

    public:

        MainWindow(QWidget *parent = nullptr);

    public slots:

        void tryToConnect();

        void limitIPSize();

    private:

        void insertInfos(QString info);

        bool m_isConnected;
        TcpConnection m_connection;

        QWidget m_mainWidget;

        QHBoxLayout m_mainLayout;

            QTabWidget m_tabs;

            QVBoxLayout m_connectionLayout;

                QTextEdit m_connectionInfos;

                QHBoxLayout m_connectionInteraction;
                    QPushButton m_connectionButton;
                    QPushButton m_disconnectionButton;
                    QPlainTextEdit m_connectionIP;
};

#endif
