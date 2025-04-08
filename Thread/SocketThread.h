#ifndef SOCKETTHREAD_H
#define SOCKETTHREAD_H

#include "../Socket/Base_AsioSocket.h"
#include "ThreadBase.h"
#include <memory>

class SocketThread : public ThreadBase
{
public:
    explicit SocketThread(const SocketInfo& socketInfo);
    ~SocketThread() override;

protected:
    void createSocket();
    const SocketInfo socketInfo;
    std::unique_ptr<unsigned char[]> buf;
    std::unique_ptr<Base_AsioSocket> socket;
};

#endif //SOCKETTHREAD_H
