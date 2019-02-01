#ifndef RESEARCH_H
#define RESEARCH_H

#include <stack>
#include <map>
#include <vector>

#include "Request.hpp"
#include "Converter.hpp"

#include <iostream>

enum class OperationType{BeginSearch, Actualize, ApplyConstraints};
enum class ConstraintType{Different, Equal, LessThan, LessOrEqual, MoreThan, MoreOrEqual};
enum class Operand{LastValue, SelectedValue};

struct Constraint {

    ConstraintType type;
    Operand operand;
    uint32_t selectedValue;

};

struct Operation {

    Operation(OperationType type):
        operationType{type} {}

    OperationType operationType;
    std::vector<Constraint> constraints;

};

struct Value {

    uint32_t address, currentValue, oldValue;
};

class Research {

    public:

        Research();

        std::vector<Request*> startResearch(const uint32_t beginAddress, const uint32_t endAddress, const uint8_t nbBytes);
        std::vector<Request*> actualizeValues();

        void resetResearch();

        void newValues(Request &request);
        bool isWaitingValues() const;

        unsigned int addConstraint(const Constraint &constraint);
        void deleteConstraint(const unsigned int constraintNumber);
        std::vector<Constraint> getCurrentConstraints();

        void applyConstraints();
        void undo();

        std::vector<Value> getCurrentValues() const;

        float getPercentResearch() const;

    private:

        bool evalConstraint(const std::pair<uint32_t, uint32_t> value) const;

        uint8_t m_nbWorkingBytes;
        uint32_t m_beginAddress, m_endAddress;
        std::stack<std::map<uint32_t, std::pair<uint32_t, uint32_t>>> m_currentSearch; //First: current value, Second: old Value
        std::stack<Operation> m_operations;
        std::vector<Constraint> m_constraints;

        bool m_isWaitingValues;
        uint32_t m_nbMissingValues;
        uint32_t m_totalWaitingValues;
};

#endif
