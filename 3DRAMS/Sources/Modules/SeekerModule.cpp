#include "SeekerModule.hpp"

SeekerModule::SeekerModule(RequestTransmitter &transmitter, QWidget *parent):
    AbstractModule{transmitter, parent},
    m_maxLinesPerColumn{13} {

    m_drawResultTable.verticalHeader()->setVisible(false);
    m_drawResultTable.horizontalHeader()->setVisible(false);


    m_newResearchGroupBox.setTitle(tr("New research"));
    m_startSearchButton.setText(tr("Start search"));
    m_resetSearchButton.setText(tr("Reset search"));
    m_resetSearchButton.setEnabled(false);
    m_fromLabel.setText(tr("From: "));
    m_toLabel.setText(tr("To: "));


    m_selectNbByteGroupBox.setTitle(tr("Result size"));
    m_oneByteCheckBox.setText("1 byte");
    m_twoByteCheckBox.setText("2 bytes");
    m_fourByteCheckBox.setText("4 bytes");
    m_fourByteCheckBox.setChecked(true);

    m_freezeButton.setText(tr("Freeze game"));
    m_resumeButton.setText(tr("Resume game"));
    m_resumeButton.setEnabled(false);
    m_actualizeButton.setText(tr("Actualize datas"));
    m_actualizeButton.setEnabled(false);

    m_applyConstraintButton.setText(tr("Apply constraints"));
    m_applyConstraintButton.setEnabled(false);
    m_deleteConstraintButton.setText(tr("Delete selected constraint"));
    m_deleteConstraintButton.setEnabled(false);
    m_addConstraintButton.setText(tr("Add constraint"));

    m_newConstraintGroupBox.setTitle(tr("New constraint"));

    m_operationGroupBox.setTitle(tr("Constraint type"));
	m_differentCheckBox.setText("!=");
    m_differentCheckBox.setChecked(true);
	m_equalCheckBox.setText("==");
    m_lessThanCheckBox.setText("<");
    m_lessOrEqualCheckBox.setText("<=");
    m_moreThanCheckBox.setText(">");
    m_moreOrEqualCheckBox.setText(">=");

    m_toCompareGroupBox.setTitle(tr("Compare to"));
    m_compareToOlderCheckBox.setText("Last value");
    m_compareToOlderCheckBox.setChecked(true);
    m_compareToValueCheckBox.setText("");

                    m_searchRangeLayout.addWidget(&m_fromLabel);
                    m_searchRangeLayout.addWidget(&m_beginSearchHexa);
                    m_searchRangeLayout.addWidget(&m_toLabel);
                    m_searchRangeLayout.addWidget(&m_endSearchHexa);

                m_startSearchLayout.addLayout(&m_searchRangeLayout);

                        m_byteLayout.addWidget(&m_oneByteCheckBox);
                        m_byteLayout.addWidget(&m_twoByteCheckBox);
                        m_byteLayout.addWidget(&m_fourByteCheckBox);

                    m_selectNbByteGroupBox.setLayout(&m_byteLayout);

                m_startSearchLayout.addWidget(&m_selectNbByteGroupBox);
                m_startSearchLayout.addWidget(&m_startSearchButton);
                m_startSearchLayout.addWidget(&m_resetSearchButton);

            m_newResearchGroupBox.setLayout(&m_startSearchLayout);

        m_searchLayout.addWidget(&m_newResearchGroupBox);

            m_gameDatasLayout.addWidget(&m_actualizeButton);
            m_gameDatasLayout.addWidget(&m_freezeButton);
            m_gameDatasLayout.addWidget(&m_resumeButton);

        m_searchLayout.addLayout(&m_gameDatasLayout);

            m_constraintLayout.addWidget(&m_applyConstraintButton);
            m_constraintLayout.addWidget(&m_deleteConstraintButton);

        m_searchLayout.addLayout(&m_constraintLayout);

        m_searchLayout.addWidget(&m_constraintList);

                        m_constraintChoiceGridLayout.addWidget(&m_differentCheckBox, 0, 0);
                    	m_constraintChoiceGridLayout.addWidget(&m_equalCheckBox, 0, 1);
                        m_constraintChoiceGridLayout.addWidget(&m_lessThanCheckBox, 1, 0);
                        m_constraintChoiceGridLayout.addWidget(&m_lessOrEqualCheckBox, 1, 1);
                        m_constraintChoiceGridLayout.addWidget(&m_moreThanCheckBox, 2, 0);
                        m_constraintChoiceGridLayout.addWidget(&m_moreOrEqualCheckBox, 2, 1);

                    m_operationGroupBox.setLayout(&m_constraintChoiceGridLayout);

                m_newConstraintGrid.addWidget(&m_operationGroupBox, 0, 0);

                        m_comparisonGridLayout.addWidget(&m_compareToOlderCheckBox, 0, 1, 1, 2);
                        m_comparisonGridLayout.addWidget(&m_compareToValueCheckBox, 1, 1);
                        m_comparisonGridLayout.addWidget(&m_valueToCompareHexa, 1, 2);

                    m_toCompareGroupBox.setLayout(&m_comparisonGridLayout);

                m_newConstraintGrid.addWidget(&m_toCompareGroupBox, 0, 1);
                m_newConstraintGrid.addWidget(&m_addConstraintButton, 1, 0, 1, 2);

            m_newConstraintGroupBox.setLayout(&m_newConstraintGrid);

        m_searchLayout.addWidget(&m_newConstraintGroupBox);

    m_mainLayout.addWidget(&m_drawResultTable, 3);
    m_mainLayout.addLayout(&m_searchLayout, 1);

    setLayout(&m_mainLayout);

    connect(&m_freezeButton, &QPushButton::clicked, this, &SeekerModule::freezeRequest);
    connect(&m_resumeButton, &QPushButton::clicked, this, &SeekerModule::resumeRequest);
    connect(&m_startSearchButton, &QPushButton::clicked, this, &SeekerModule::startRequest);
    connect(&m_resetSearchButton, &QPushButton::clicked, this, &SeekerModule::resetResearch);
    connect(&m_actualizeButton, &QPushButton::clicked, this, &SeekerModule::actualizeRequest);
    connect(&m_applyConstraintButton, &QPushButton::clicked, this, &SeekerModule::applyConstraints);
    connect(&m_deleteConstraintButton, &QPushButton::clicked, this, &SeekerModule::deleteSelectedConstraint);
    connect(&m_addConstraintButton, &QPushButton::clicked, this, &SeekerModule::addConstraint);
}

SeekerModule::~SeekerModule() {}

void SeekerModule::newResult(Request result) {

    switch(result.type) {

        case RequestType::FreezeGame:
            if(result.parameters[0] == 0) {

                m_freezeButton.setEnabled(true);
                m_resumeButton.setEnabled(false);
            }

            else {

                m_freezeButton.setEnabled(false);
                m_resumeButton.setEnabled(true);
            }

            break;

        case RequestType::GetSetOfValue: {

            m_currentResearch.newValues(result);
            if(!m_currentResearch.isWaitingValues()) { 

                resumeRequest(); 
                refreshTable();
            }

            //Send % to information

            float waitingPercent{m_currentResearch.getPercentResearch()};
            std::cout << waitingPercent << " %" << std::endl;
            //Need to convert float into uint32

            uint32_t *convertedPercent{reinterpret_cast<uint32_t*>(&waitingPercent)};

            Request *percentRequest{newRequest(Module::Informations, RequestType::ResearchProgress)};
            writeNumber(percentRequest, *convertedPercent);

            m_transmitter.addRequest(percentRequest);
        }

            break;

        default:
            break;
    }
}

void SeekerModule::resetResearch() {

    resumeRequest();

    m_currentResearch.resetResearch();
    refreshTable();

    m_startSearchButton.setEnabled(true);
    m_beginSearchHexa.setEnabled(true);
    m_endSearchHexa.setEnabled(true);
    m_oneByteCheckBox.setEnabled(true);
    m_twoByteCheckBox.setEnabled(true);
    m_fourByteCheckBox.setEnabled(true);

    m_resetSearchButton.setEnabled(false);
    m_actualizeButton.setEnabled(false);
    m_applyConstraintButton.setEnabled(false);
}

void SeekerModule::addConstraint() {

    std::string constraintName;

    Constraint newConstraint;

    if(m_differentCheckBox.isChecked()) { newConstraint.type = ConstraintType::Different; constraintName += "== "; }
    else if(m_equalCheckBox.isChecked()) { newConstraint.type = ConstraintType::Equal; constraintName += "!= "; }
    else if(m_lessThanCheckBox.isChecked()) { newConstraint.type = ConstraintType::LessThan; constraintName += "< "; }
    else if(m_lessOrEqualCheckBox.isChecked()) { newConstraint.type = ConstraintType::LessOrEqual; constraintName += "<= "; }
    else if(m_moreThanCheckBox.isChecked()) { newConstraint.type = ConstraintType::MoreThan; constraintName += "> "; }
    else if(m_moreOrEqualCheckBox.isChecked()) { newConstraint.type = ConstraintType::MoreOrEqual; constraintName += ">= "; }

    if(m_compareToOlderCheckBox.isChecked()) { 

        newConstraint.operand = Operand::LastValue;
        newConstraint.selectedValue = 0;
        constraintName += " old value";
    }

    else {

        newConstraint.operand = Operand::SelectedValue;
        newConstraint.selectedValue = m_valueToCompareHexa.getValue();
        char hexaString[8];
        hexaNumberToString(m_valueToCompareHexa.getValue(), hexaString);
        constraintName += hexaString;
    }

    m_currentResearch.addConstraint(newConstraint);

    m_deleteConstraintButton.setEnabled(true);

    std::cout << "Adding " << constraintName << std::endl;
    m_constraintList.addItem(QString::fromStdString(constraintName));
}


void SeekerModule::applyConstraints() {

    m_currentResearch.applyConstraints();
    refreshTable();
}

void SeekerModule::deleteSelectedConstraint() {

    //Delete selected constraint

    int selectedConstraint{m_constraintList.currentRow()};

    m_currentResearch.deleteConstraint(static_cast<unsigned int>(selectedConstraint));

    delete m_constraintList.takeItem(selectedConstraint);
    //Delete required by Qt

    if(m_currentResearch.getCurrentConstraints().empty()) { m_deleteConstraintButton.setEnabled(false); }
}



void SeekerModule::actualizeRequest() {

    freezeRequest();

    std::vector<Request*> requestToSend{m_currentResearch.actualizeValues()};
    for(Request* currentRequest: requestToSend) { m_transmitter.addRequest(currentRequest); }
}


void SeekerModule::freezeRequest() {

    //0 -> resume
    Request *request{newRequest(Module::Seeker, RequestType::FreezeGame)};
    writeNumber(request, 1);

    m_transmitter.addRequest(request);
}

void SeekerModule::resumeRequest() {

    //other -> freeze
    Request *request{newRequest(Module::Seeker, RequestType::FreezeGame)};
    writeNumber(request, 0);

    m_transmitter.addRequest(request);
}

void SeekerModule::startRequest() {

    const uint32_t beginValue{m_beginSearchHexa.getValue()}, endValue{m_endSearchHexa.getValue()};

    if(beginValue <= endValue) {

        m_startSearchButton.setEnabled(false);
        m_beginSearchHexa.setEnabled(false);
        m_endSearchHexa.setEnabled(false);
        m_oneByteCheckBox.setEnabled(false);
        m_twoByteCheckBox.setEnabled(false);
        m_fourByteCheckBox.setEnabled(false);
            
        m_resetSearchButton.setEnabled(true);
        m_actualizeButton.setEnabled(true);
        m_applyConstraintButton.setEnabled(true);

        std::vector<Request*> requestToSend{m_currentResearch.startResearch(beginValue, endValue, 4)};

        freezeRequest();

        for(Request* currentRequest: requestToSend) { m_transmitter.addRequest(currentRequest); }
    }

    //Else: message d'erreur pour Informations
}

void SeekerModule::refreshTable() {

    std::vector<Value> values{m_currentResearch.getCurrentValues()};

    unsigned int currentColumn{0}, currentRow{1};
                 
    const long unsigned int rowCount{1 + std::min(static_cast<long unsigned int>(m_maxLinesPerColumn), values.size())}, 
                            columnCount{3 + 3*(values.size()/m_maxLinesPerColumn)};

   // m_drawResultTable.clear();
    //m_headerTableItems.clear();

    m_drawResultTable.setRowCount(rowCount);
    //Fait planté ! SIGSEV sur malloc
    m_drawResultTable.setColumnCount(columnCount);

    for(unsigned int i{0}; i < columnCount/3; i++) { 

        QTableWidgetItem *addressHeader = new QTableWidgetItem{};
        addressHeader->setText(tr("Address"));
        addressHeader->setTextAlignment(Qt::AlignCenter);
        addressHeader->setFlags(Qt::ItemIsEnabled);
        addressHeader->setBackground(QBrush{QColor{175, 175, 175}});
        m_drawResultTable.setItem(0, i*3 + 0, addressHeader);

        QTableWidgetItem *currentValueHeader = new QTableWidgetItem{};
        currentValueHeader->setText(tr("Current value"));
        currentValueHeader->setTextAlignment(Qt::AlignCenter);
        currentValueHeader->setFlags(Qt::ItemIsEnabled);
        currentValueHeader->setBackground(QBrush{QColor{0, 230, 230}});
        m_drawResultTable.setItem(0, i*3 + 1, currentValueHeader);

        QTableWidgetItem *previousValueHeader = new QTableWidgetItem{};
        previousValueHeader->setText(tr("Previous Value"));
        previousValueHeader->setTextAlignment(Qt::AlignCenter);
        previousValueHeader->setFlags(Qt::ItemIsEnabled);
        previousValueHeader->setBackground(QBrush{QColor{0, 175, 175}});
        m_drawResultTable.setItem(0, i*3 + 2, previousValueHeader);
    }

    char currentString[8];

    //Values items
    for(unsigned int i{0}; i < values.size(); i++) {

        hexaNumberToString(values[i].address, currentString);
        QTableWidgetItem *address = new QTableWidgetItem{};
        address->setText(QString{currentString});
        address->setTextAlignment(Qt::AlignCenter);
        address->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        address->setBackground(QBrush{QColor{200, 200, 200}});
        m_drawResultTable.setItem(currentRow, currentColumn, address);

        hexaNumberToString(values[i].currentValue, currentString);
        QTableWidgetItem *currentValue = new QTableWidgetItem{};
        currentValue->setText(QString{currentString});
        currentValue->setTextAlignment(Qt::AlignCenter);
        currentValue->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        currentValue->setBackground(QBrush{QColor{0, 255, 255}});
        m_drawResultTable.setItem(currentRow, currentColumn+1, currentValue);

        hexaNumberToString(values[i].oldValue, currentString);
        QTableWidgetItem *oldValue = new QTableWidgetItem{};
        oldValue->setText(QString{currentString});
        oldValue->setTextAlignment(Qt::AlignCenter);
        oldValue->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
        oldValue->setBackground(QBrush{QColor{0, 200, 200}});
        m_drawResultTable.setItem(currentRow, currentColumn+2, oldValue);

        currentRow++;
        if(currentRow >= rowCount) {

            currentRow = 1;
            currentColumn += 3; 
        }
    }
}

