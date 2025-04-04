#ifndef TXTHREAD_H
#define TXTHREAD_H

#include "SocketThread.h"
#include <chrono>

class TxThread : public SocketThread
{
public:
    TxThread(const SocketInfo& socketInfo, const std::chrono::milliseconds sleep_time)
        : SocketThread {socketInfo}
    , sleep_time {sleep_time}
    {}

    ~TxThread() override = default;

private:
    void run() override;

    const std::chrono::milliseconds sleep_time;
};

#endif //TXTHREAD_H
