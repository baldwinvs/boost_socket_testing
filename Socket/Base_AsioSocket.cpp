#include "Base_AsioSocket.h"

Base_AsioSocket::Base_AsioSocket(const SocketInfo &socketInfo)
    : ioContext {}
    , socketInfo {socketInfo}
{

}
