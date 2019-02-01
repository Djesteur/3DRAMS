#include "SurveilModule.hpp"

SurveilModule::SurveilModule(RequestTransmitter &transmitter, QWidget *parent):
    AbstractModule{transmitter, parent} {

    m_sendButton.setText(tr("Get Value"));

    m_resultText.setText("00000000");
    m_resultText.setReadOnly(true);

    m_mainLayout.addWidget(&m_addressText);
    m_mainLayout.addWidget(&m_sendButton);
    m_mainLayout.addWidget(&m_resultText);

    setLayout(&m_mainLayout);

    connect(&m_sendButton, &QPushButton::clicked, this, &SurveilModule::getNewValue);
}

SurveilModule::~SurveilModule() {}

void SurveilModule::newResult(Request result) {

    switch(result.type) {

        default:
            case RequestType::GetValue:

            char newValue[8];
            hexaNumberToString(parametersToNumber(&(result.parameters[0])), newValue);
            m_resultText.setText(QString{newValue});
            break;
    }
}

void SurveilModule::getNewValue() {

	Request *request{newRequest(Module::Surveil, RequestType::GetValue)};
	writeNumber(request, m_addressText.getValue());

    m_transmitter.addRequest(request);
}