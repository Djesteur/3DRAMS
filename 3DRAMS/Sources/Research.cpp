#include "Research.hpp"

Research::Research():
	m_nbWorkingBytes{0},
	m_beginAddress{0},
	m_endAddress{0},
	m_isWaitingValues{false},
	m_nbMissingValues{0},
    m_totalWaitingValues{0} {}


std::vector<Request*> Research::startResearch(const uint32_t beginAddress, const uint32_t endAddress, const uint8_t nbBytes) {

	m_currentSearch.emplace();
	m_operations.emplace(OperationType::BeginSearch);

	m_nbWorkingBytes = nbBytes;
	m_beginAddress = beginAddress;
	m_endAddress = endAddress;

	std::vector<Request*> requestToSend;

	uint16_t nbMaxValuePerRequest{static_cast<uint16_t>(MAX_PARAMETERS_SIZE-1)};
    nbMaxValuePerRequest /= (4 + m_nbWorkingBytes); // -1 for working bytes, +4 for each address
    uint32_t nbValueInRequest{0}, currentValueToAdd{m_beginAddress};

    while(currentValueToAdd <= m_endAddress) {

        nbValueInRequest = 0;

        Request *request{newRequest(Module::Seeker, RequestType::GetSetOfValue)};

        writeByte(request, m_nbWorkingBytes);

        while(nbValueInRequest < nbMaxValuePerRequest && currentValueToAdd <= m_endAddress) {

            writeNumber(request, currentValueToAdd);
            currentValueToAdd += m_nbWorkingBytes;
            ++nbValueInRequest;
            ++m_nbMissingValues;
        }

        requestToSend.emplace_back(request);
    }

    m_isWaitingValues = true;
    m_totalWaitingValues = m_nbMissingValues;

    return requestToSend;
}

std::vector<Request*> Research::actualizeValues() {

    m_operations.emplace(OperationType::Actualize);

    std::vector<Request*> requestToSend;

    uint16_t nbMaxValuePerRequest{static_cast<uint16_t>(MAX_PARAMETERS_SIZE-1)};
    nbMaxValuePerRequest /= (4 + m_nbWorkingBytes); // -1 for working bytes, +4 for each address
    uint32_t nbValueInRequest{0}, currentAddress{0};

    std::vector<uint32_t> addressToActualize;

    for(std::map<uint32_t, std::pair<uint32_t, uint32_t>>::const_iterator it = m_currentSearch.top().begin(); it != m_currentSearch.top().end(); it++) {

        addressToActualize.emplace_back(it->first);
    }

    while(currentAddress < addressToActualize.size()) {

        nbValueInRequest = 0;

        Request *request{newRequest(Module::Seeker, RequestType::GetSetOfValue)};

        writeByte(request, m_nbWorkingBytes);

        while(nbValueInRequest < nbMaxValuePerRequest && currentAddress <= addressToActualize.size()) {

            writeNumber(request, addressToActualize[currentAddress]);
            ++nbValueInRequest;
            ++m_nbMissingValues;
            ++currentAddress;
        }

        requestToSend.emplace_back(request);
    }

    m_isWaitingValues = true;
    m_totalWaitingValues = m_nbMissingValues;

    return requestToSend;
}

void Research::resetResearch() {

    m_nbWorkingBytes = 0;
    m_beginAddress = 0;
    m_endAddress = 0;
    while(!m_currentSearch.empty()) { m_currentSearch.pop(); }
    while(!m_operations.empty()) { m_operations.pop(); }

    m_isWaitingValues = false;
    m_nbMissingValues = 0;
    m_totalWaitingValues = 0;
}


void Research::newValues(Request &request) {

	uint32_t nbValueReceived{0}, lastAddress{0}, lastValue{0};
	uint16_t nextParameter{0};

    while(nbValueReceived < request.nbParameters/(4u + m_nbWorkingBytes)) { // -1 for working bytes, +4 for each address

        lastAddress = parametersToNumber(&(request.parameters[nextParameter]));
        lastValue = parametersToNumber(&(request.parameters[nextParameter + m_nbWorkingBytes]));

        if(m_currentSearch.top().find(lastAddress) == m_currentSearch.top().end()) { 

            m_currentSearch.top().insert(std::make_pair(lastAddress, std::make_pair(lastValue, lastValue))); 
        }

        else {

            m_currentSearch.top()[lastAddress].second = m_currentSearch.top()[lastAddress].first;
            m_currentSearch.top()[lastAddress].first = lastValue;
        }

        --m_nbMissingValues;
        ++nbValueReceived;
        nextParameter += 4 + m_nbWorkingBytes;
    }

    if(m_nbMissingValues == 0) { m_isWaitingValues = false; }
}

bool Research::isWaitingValues() const { return m_isWaitingValues; }


unsigned int Research::addConstraint(const Constraint &constraint) {

    m_constraints.emplace_back(constraint);
    return m_constraints.size() -1;
}

void Research::deleteConstraint(const unsigned int constraintNumber) {

    if(constraintNumber < m_constraints.size()) { m_constraints.erase(m_constraints.begin() + constraintNumber); }
}

std::vector<Constraint> Research::getCurrentConstraints() { return std::vector<Constraint>{};}


void Research::applyConstraints() {

    m_operations.emplace(OperationType::ApplyConstraints);

    m_currentSearch.emplace(m_currentSearch.top());


    std::vector<std::map<uint32_t, std::pair<uint32_t, uint32_t>>::iterator> valuesToDelete;

    for(std::map<uint32_t, std::pair<uint32_t, uint32_t>>::iterator it = m_currentSearch.top().begin(); it != m_currentSearch.top().end(); it++) {

       if(!evalConstraint(it->second)) { valuesToDelete.emplace_back(it); }
    }

    for(std::map<uint32_t, std::pair<uint32_t, uint32_t>>::iterator it: valuesToDelete) { m_currentSearch.top().erase(it); }

}

void Research::undo() {

    m_operations.pop();
    m_currentSearch.pop();
}

std::vector<Value> Research::getCurrentValues() const {

    std::vector<Value> result;
    Value newValue;

    if(!m_currentSearch.empty()) {
        if(!m_currentSearch.top().empty()) {

            for(std::pair<uint32_t, std::pair<uint32_t, uint32_t>> currentAddress: m_currentSearch.top()) {

                newValue.address = currentAddress.first;
                newValue.currentValue = currentAddress.second.first;
                newValue.oldValue = currentAddress.second.second;
                result.emplace_back(newValue);
            }
        }
    }

    return result;
}

bool Research::evalConstraint(const std::pair<uint32_t, uint32_t> value) const {

    uint32_t currentOperand;

    for(const Constraint &currentConstraint: m_constraints) {

        if(currentConstraint.operand == Operand::LastValue) { currentOperand = value.second; }
        else { currentOperand = currentConstraint.selectedValue; }

        switch(currentConstraint.type) {

            case ConstraintType::Equal: 
                if(value.first != currentOperand) { return false; }
                break;

            case ConstraintType::Different: 
                if(value.first == currentOperand) { return false; }
                break;

            case ConstraintType::LessThan: 
                if(value.first >= currentOperand) { return false; }
                break;

            case ConstraintType::LessOrEqual: 
                if(value.first > currentOperand) { return false; }
                break;

            case ConstraintType::MoreThan:
                if(value.first <= currentOperand) { return false; }
                break;

            case ConstraintType::MoreOrEqual: 
                if(value.first < currentOperand) { return false; }
                break;
        }
    }

    return true;
}


float Research::getPercentResearch() const {  return (static_cast<float>(m_totalWaitingValues) - static_cast<float>(m_nbMissingValues))*100.f/static_cast<float>(m_totalWaitingValues); }