#ifndef RXTHREAD_H
#define RXTHREAD_H

#include "SocketThread.h"

class RxThread : public SocketThread
{
public:
    explicit RxThread(const SocketInfo& socketInfo)
        : SocketThread {socketInfo}
    {}

    ~RxThread() override = default;
private:
    void run() override;
};

#endif //RXTHREAD_H
