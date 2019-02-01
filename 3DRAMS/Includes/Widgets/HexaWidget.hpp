#ifndef HEXAWIDGET_HPP
#define HEXAWIDGET_HPP

#include <QLineEdit>

#include "Converter.hpp"

class HexaWidget : public QLineEdit {

    Q_OBJECT

    public:

        HexaWidget(QWidget *parent = nullptr);
        virtual ~HexaWidget();

        uint32_t getValue();
};

#endif
