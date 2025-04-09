#ifndef SOCKETINFO_H
#define SOCKETINFO_H

#include <cstdint>
#include <string>

enum class SocketType
{
    udp,
    tcp
};

enum class SocketProperties
{
    //Format: type_receive/send_blocking/nonblocking
    udp_send_blocking,
    udp_send_nonblocking,
    udp_receive_blocking,
    udp_receive_nonblocking,
    tcp_send_blocking,
    tcp_send_nonblocking,
    tcp_receive_blocking,
    tcp_receive_nonblocking
};

struct SocketInfo
{
    SocketInfo(const std::string& ip, const uint16_t port,
        const SocketType type, const bool receiver, const bool non_blocking,
        const uint32_t bufferSize)
    : ip {ip}
    , port {port}
    , bufferSize {bufferSize}
    {
        //confusing but it works because each input represents 2 choices
        properties = static_cast<SocketProperties>(static_cast<int>(type) << 2 | static_cast<int>(receiver) << 1 | static_cast<int>(non_blocking));
    }

    std::string ip {};
    uint16_t port {};
    uint32_t bufferSize {};
    SocketProperties properties {};
};

#endif //SOCKETINFO_H
