#ifndef RECEIVESOCKETTHREAD_H
#define RECEIVESOCKETTHREAD_H

#include "SocketThread.h"
#include "../Socket/Wrapper_AsioTcp.h"
#include "../Socket/Wrapper_AsioUdp.h"

class ReceiveSocketThread : public SocketThread
{
public:
    explicit ReceiveSocketThread(const SocketInfo& info, const SocketProperties properties)
        : SocketThread {info, properties}
    {}

    ~ReceiveSocketThread() override;
    void set_nonblocking_poll_time(const std::chrono::milliseconds& poll_time_ms = std::chrono::milliseconds(0));
protected:
    virtual void receiveCallback(const size_t bytes);

private:
    void run() override;
    void yield() const;
    std::chrono::milliseconds poll_time_ms {};
};

#endif //RECEIVESOCKETTHREAD_H
