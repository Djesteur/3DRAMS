#include "HexaWidget.hpp" 

HexaWidget::HexaWidget(QWidget *parent):
	QLineEdit{parent} {

	setInputMask("HHHHHHHH");
	setText("00000000");
	setCursorPosition(0);
	setMaxLength(8);
	setFixedHeight(25);
    setFixedWidth(70);
}

HexaWidget::~HexaWidget() {}

uint32_t HexaWidget::getValue() {

	std::string value{text().toUtf8().constData()};
	while(value.size() < 8) { value.insert(0, "0"); }
    const char *numberToConvert{value.c_str()};

	return stringToHexaNumber(numberToConvert);
}