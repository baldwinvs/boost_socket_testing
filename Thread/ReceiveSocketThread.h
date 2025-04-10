#ifndef RECEIVESOCKETTHREAD_H
#define RECEIVESOCKETTHREAD_H

#include "SocketThread.h"
#include "../Socket/Wrapper_AsioTcp.h"
#include "../Socket/Wrapper_AsioUdp.h"

class ReceiveSocketThread : public SocketThread
{
public:
    explicit ReceiveSocketThread(const SocketInfo& socketInfo)
        : SocketThread {socketInfo}
    {}

    ~ReceiveSocketThread() override;
protected:
    virtual void receiveCallback(const size_t bytes);

private:
    void run() override;
};

#endif //RECEIVESOCKETTHREAD_H
