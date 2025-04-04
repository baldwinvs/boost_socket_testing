#ifndef RECEIVESOCKETTHREAD_H
#define RECEIVESOCKETTHREAD_H

#include "SocketThread.h"

class ReceiveSocketThread : public SocketThread
{
public:
    explicit ReceiveSocketThread(const SocketInfo& socketInfo)
        : SocketThread {socketInfo}
    {}

    ~ReceiveSocketThread() override = default;
protected:
    virtual void receiveCallback(const size_t bytes) = 0;

private:
    void run() override;
};

#endif //RECEIVESOCKETTHREAD_H
