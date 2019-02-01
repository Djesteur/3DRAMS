#ifndef SEEKERMODULE_HPP
#define SEEKERMODULE_HPP

#include <stack>
#include <map>
#include <algorithm> 

#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QLabel>
#include <QGridLayout>
#include <QListWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QGroupBox>

#include "AbstractModule.hpp"
#include "HexaWidget.hpp"

#include "Research.hpp"

//First: current value
//Second: old value

class SeekerModule: public AbstractModule {

    Q_OBJECT

    public:

        SeekerModule(RequestTransmitter &transmitter, QWidget *parent = nullptr);

        virtual ~SeekerModule();

    public slots:

        virtual void newResult(Request result);

        void resetResearch();
        void addConstraint();

        void applyConstraints();
        void deleteSelectedConstraint();

        void freezeRequest();
        void resumeRequest();
        void startRequest();
        void actualizeRequest();

    private:

        void refreshTable();

        QVBoxLayout m_mainLayout;
            // affichage des données

            QTableWidget m_drawResultTable;
            QHBoxLayout m_searchLayout;

                QGroupBox m_newResearchGroupBox;
                    QVBoxLayout m_startSearchLayout;
                        QHBoxLayout m_searchRangeLayout;
                            QLabel m_fromLabel;
                            QLabel m_toLabel;
                            HexaWidget m_beginSearchHexa;
                            HexaWidget m_endSearchHexa;
                        QGroupBox m_selectNbByteGroupBox;
                            QHBoxLayout m_byteLayout;
                                QRadioButton m_oneByteCheckBox;
                                QRadioButton m_twoByteCheckBox;
                                QRadioButton m_fourByteCheckBox;
                        QPushButton m_startSearchButton;
                        QPushButton m_resetSearchButton;

                QVBoxLayout m_gameDatasLayout;
                    QPushButton m_freezeButton;
                    QPushButton m_resumeButton;
                    QPushButton m_actualizeButton;

                QVBoxLayout m_constraintLayout;
                
                    QPushButton m_applyConstraintButton;
                    QPushButton m_deleteConstraintButton;

                QListWidget m_constraintList;

                QGroupBox m_newConstraintGroupBox;
                    QGridLayout m_newConstraintGrid;
                        
                        QGroupBox m_operationGroupBox;
                            QGridLayout m_constraintChoiceGridLayout;
                                QRadioButton m_differentCheckBox;
                                QRadioButton m_equalCheckBox;
                                QRadioButton m_lessThanCheckBox;
                                QRadioButton m_lessOrEqualCheckBox;
                                QRadioButton m_moreThanCheckBox;
                                QRadioButton m_moreOrEqualCheckBox;


                        QGroupBox m_toCompareGroupBox;
                            QGridLayout m_comparisonGridLayout; 
                                QRadioButton m_compareToOlderCheckBox;
                                QRadioButton m_compareToValueCheckBox;
                                HexaWidget m_valueToCompareHexa;

                        QPushButton m_addConstraintButton;

        Research m_currentResearch;

        const unsigned int m_maxLinesPerColumn;
};

#endif
