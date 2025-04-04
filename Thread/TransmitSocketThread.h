#ifndef TRANSMITSOCKETTHREAD_H
#define TRANSMITSOCKETTHREAD_H

#include "SocketThread.h"
#include <chrono>

class TransmitSocketThread : public SocketThread
{
public:
    TransmitSocketThread(const SocketInfo& socketInfo, const std::chrono::milliseconds sleep_time)
        : SocketThread {socketInfo}
    , sleep_time {sleep_time}
    {}

    ~TransmitSocketThread() override = default;

private:
    void run() override;

    const std::chrono::milliseconds sleep_time;
};

#endif //TRANSMITSOCKETTHREAD_H
