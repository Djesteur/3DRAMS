#ifndef REQUESTTRANSMITTER_HPP
#define REQUESTTRANSMITTER_HPP

#include <queue>
#include <mutex>

#include "Request.hpp"

class RequestTransmitter {

    public:

        RequestTransmitter();
        ~RequestTransmitter();

        RequestTransmitter(const RequestTransmitter&) = delete;
        RequestTransmitter &operator=(const RequestTransmitter&) = delete;

        bool waitingRequest();
        void addRequest(Request *request);
        Request* getLastRequest();

        bool waitingResult();
        void addResult(Request *result);
        Request* getLastResult();

    private:

        std::queue<Request*> m_request, m_result;
        std::mutex m_requestMutex, m_resultMutex;
};

#endif
