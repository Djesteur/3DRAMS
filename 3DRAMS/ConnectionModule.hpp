#ifndef CONNECTIONMODULE_HPP
#define CONNECTIONMODULE_HPP

#include <sstream>

#include <QHBoxLayout>

#include <QTabWidget>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QTextEdit>


#include "AbstractModule.hpp"


class ConnectionModule : public AbstractModule {

    Q_OBJECT

    public:

        ConnectionModule(ConnectionToPlugin &connectionToPlugin, QWidget *parent = nullptr);

        virtual ~ConnectionModule();

    public slots:

        virtual void newResult(Request result);

        void limitIPSize();
        void connectButtonSlot();
        void disconnectButtonSlot();

    private:

        QHBoxLayout m_mainLayout;
            QPlainTextEdit m_connectionIP;
            QPushButton m_connectionButton;
            QPushButton m_disconnectionButton;

        const int m_maxIPChar;
};

std::vector<std::string> splitDatas(const std::string &datas, const char spliter);

#endif
