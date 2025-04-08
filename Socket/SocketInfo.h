#ifndef SOCKETINFO_H
#define SOCKETINFO_H

#include <cstdint>
#include <string>

enum class SocketType
{
    udp,
    tcp
};

struct SocketInfo
{
    SocketInfo(const std::string& ip, const uint16_t port,
        const SocketType type, const bool receiver, const bool non_blocking,
        const uint32_t bufferSize)
    : ip {ip}
    , port {port}
    , type {type}
    , receiver {receiver}
    , non_blocking {non_blocking}
    , bufferSize {bufferSize}
    {}

    std::string ip {};
    uint16_t port {};
    SocketType type {};
    bool receiver {};
    bool non_blocking {};
    uint32_t bufferSize {};
};

#endif //SOCKETINFO_H
