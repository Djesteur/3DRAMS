#ifndef HEXAWIDGET_HPP
#define SURVEILMODULE_HPP

#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>

#include "AbstractModule.hpp"

class SurveilModule : public AbstractModule {

    Q_OBJECT

    public:

        SurveilModule(RequestTransmitter &m_transmitter, QWidget *parent = nullptr);
        virtual ~SurveilModule();

   public slots:

        virtual void newResult(Request result);
        virtual void getNewValue();

    private:

        void newCharEntered();

        QHBoxLayout m_mainLayout;

            QPlainTextEdit m_addressText;
            QPushButton m_sendButton;
            QPlainTextEdit m_resultText;

       const unsigned int m_maxAddressChar;
};

#endif
