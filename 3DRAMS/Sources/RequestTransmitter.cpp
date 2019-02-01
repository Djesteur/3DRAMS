#include "RequestTransmitter.hpp"

RequestTransmitter::RequestTransmitter() {}

RequestTransmitter::~RequestTransmitter() {

	m_requestMutex.lock();
	while(!m_request.empty()) {

		freeRequest(m_request.front());
		m_request.pop();
	}
	m_requestMutex.unlock();

	m_resultMutex.lock();
	while(!m_result.empty()) {

		freeRequest(m_result.front());
		m_result.pop();
	}
	m_resultMutex.unlock();
}


bool RequestTransmitter::waitingRequest() { return !m_request.empty(); }

void RequestTransmitter::addRequest(Request *request) {

	if(request->requester == Module::Informations) { addResult(request); } //Allow to send information directly to Information module
	else {

		m_requestMutex.lock();
		m_request.emplace(request);
		m_requestMutex.unlock();
	}
}

Request* RequestTransmitter::getLastRequest() {

	m_requestMutex.lock();
	Request *lastRequest{m_request.front()};
	m_request.pop();
	m_requestMutex.unlock();

	return lastRequest;
}


bool RequestTransmitter::waitingResult() { return !m_result.empty(); }

void RequestTransmitter::addResult(Request *result) {

	m_resultMutex.lock();
	m_result.emplace(result);
	m_resultMutex.unlock();
}

Request* RequestTransmitter::getLastResult() {

	m_resultMutex.lock();
	Request *lastResult{m_result.front()};
	m_result.pop();
	m_resultMutex.unlock();

	return lastResult;
}
