#ifndef INFORMATIONMODULE_HPP
#define INFORMATIONMODULE_HPP

#include <QTextEdit>
#include <QHBoxLayout>
#include <QTime>

#include "AbstractModule.hpp"

class InformationModule: public AbstractModule  {

    Q_OBJECT

    public:

        InformationModule(RequestTransmitter &transmitter, QWidget *parent = nullptr);

        virtual ~InformationModule();

    public slots:

        virtual void newResult(Request result);

    private:

        void insertInfos(QString info, QColor textColor);

        void connectionInfo(Request result);
        void generalInfo(Request result);

        QHBoxLayout m_mainLayout;
            QTextEdit m_text;
};

#endif
