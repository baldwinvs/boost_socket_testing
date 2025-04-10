#ifndef TRANSMITSOCKETTHREAD_H
#define TRANSMITSOCKETTHREAD_H

#include "SocketThread.h"
#include "../Socket/Wrapper_AsioTcp.h"
#include "../Socket/Wrapper_AsioUdp.h"
#include <chrono>

class TransmitSocketThread : public SocketThread
{
public:
    TransmitSocketThread(const SocketInfo& info, const SocketProperties properties, const std::chrono::milliseconds sleep_time)
        : SocketThread {info, properties}
        , sleep_time {sleep_time}
    {}

    ~TransmitSocketThread() override = default;

private:
    void run() override;

    const std::chrono::milliseconds sleep_time;
};

#endif //TRANSMITSOCKETTHREAD_H
