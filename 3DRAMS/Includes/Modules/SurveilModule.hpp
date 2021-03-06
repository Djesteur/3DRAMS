#ifndef SURVEILMODULE_HPP
#define SURVEILMODULE_HPP

#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>

#include "AbstractModule.hpp"

#include "HexaWidget.hpp"

class SurveilModule : public AbstractModule {

    Q_OBJECT

    public:

        SurveilModule(RequestTransmitter &m_transmitter, QWidget *parent = nullptr);
        virtual ~SurveilModule();

   public slots:

        virtual void newResult(Request result);
        virtual void getNewValue();

    private:

        QHBoxLayout m_mainLayout;

            HexaWidget m_addressText;
            QPushButton m_sendButton;
            HexaWidget m_resultText;
};

#endif
