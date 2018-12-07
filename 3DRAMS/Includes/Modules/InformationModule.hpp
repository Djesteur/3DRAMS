#ifndef INFORMATIONMODULE_HPP
#define INFORMATIONMODULE_HPP

#include <QTextEdit>
#include <QHBoxLayout>

class MainWindow;

#include "AbstractModule.hpp"

class InformationModule: public AbstractModule  {

    Q_OBJECT

    public:

        InformationModule(MainWindow *mainWindow, QWidget *parent = nullptr);

        virtual ~InformationModule();

    public slots:

        virtual void newResult(Request result);

    private:

        void insertInfos(QString info);

        void connectionInfo(Request result);

        QHBoxLayout m_mainLayout;
            QTextEdit m_text;
};

#endif
