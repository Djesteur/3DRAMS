#ifndef SURVEILMODULE_HPP
#define SURVEILMODULE_HPP

#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>

class MainWindow;

#include "AbstractModule.hpp"
#include "Converter.hpp"

class SurveilModule : public AbstractModule {

    Q_OBJECT

    public:

        SurveilModule(MainWindow *mainWindow, QWidget *parent = nullptr);
        virtual ~SurveilModule();

   public slots:

        virtual void newResult(Request result);
        virtual void getNewValue();

        virtual void testSlot(QString value);

    private:

        void newCharEntered();

        QHBoxLayout m_mainLayout;

            QPlainTextEdit m_addressText;
            QPushButton m_sendButton;
            QPlainTextEdit m_resultText;

       const unsigned int m_maxAddressChar;
};

#endif
