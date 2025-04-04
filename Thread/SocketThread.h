#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include "SocketInfo.h"
#include "ThreadBase.h"
#include <memory>

class SocketThread : public ThreadBase
{
public:
    explicit SocketThread(const SocketInfo& socketInfo);
    ~SocketThread() override;

protected:
    const SocketInfo socketInfo;
    std::unique_ptr<unsigned char[]> buf;
};

#endif //SOCKETTHREAD_H
