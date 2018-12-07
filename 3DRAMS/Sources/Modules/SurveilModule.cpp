#include "MainWindow.hpp"
#include "SurveilModule.hpp"


SurveilModule::SurveilModule(MainWindow *mainWindow, QWidget *parent):
    AbstractModule{mainWindow, parent},
    m_maxAddressChar{8} {

    m_sendButton.setText(tr("Get Value"));

    m_addressText.setPlainText("3071F8B4");
    m_resultText.setPlainText("00000000");
    m_resultText.setReadOnly(true);

    m_mainLayout.addWidget(&m_addressText);
    m_mainLayout.addWidget(&m_sendButton);
    m_mainLayout.addWidget(&m_resultText);

    setLayout(&m_mainLayout);

    connect(&m_sendButton, &QPushButton::clicked, this, &SurveilModule::getNewValue);
    //connect(&m_addressText, &QPlainTextEdit::textChanged, this, &SurveilModule::newCharEntered);
}

SurveilModule::~SurveilModule() {}

void SurveilModule::newResult(Request result) {

    switch(result.type) {

        default:
            case RequestType::GetValue:

            char newValue[8];
            hexaNumberToString(parametersToNumber(&(result.parameters[0])), newValue);
            emit testSlot(QString::fromStdString(std::string{newValue}));
            break;
    }
}

void SurveilModule::testSlot(QString value) {

    //m_resultText.clear();
    m_resultText.setPlainText("SLAEEEE");
}


void SurveilModule::getNewValue() {

	Request *request{newRequest(Module::Surveil, RequestType::GetValue)};
	std::string address{m_addressText.toPlainText().toUtf8().constData()};
    const char *numberToConvert{address.c_str()};
	writeNumber(request, stringToHexaNumber(numberToConvert));

    emit m_mainWindow->newRequest(request);
}


void SurveilModule::newCharEntered() {

    if(m_addressText.toPlainText().length() > static_cast<int>(m_maxAddressChar)) { m_addressText.textCursor().deletePreviousChar();  }

	if(m_addressText.toPlainText().length() > 0) {

		std::string address{m_addressText.toPlainText().toUtf8().constData()};

		if(charToHexa(address.back()) >= 16) { m_addressText.textCursor().deletePreviousChar(); } // Not a valid number
	}

}
