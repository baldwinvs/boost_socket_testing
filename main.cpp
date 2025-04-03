#include <thread>
#include <boost/asio.hpp>

#include <array>
#include <atomic>
#include <cstdio>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>

constexpr uint16_t port {58585};
constexpr size_t bufSize {16};
inline const std::string address {"127.0.0.1"};

struct SocketInfo
{
    SocketInfo(const std::string& ip, const uint16_t port, const bool receiver, const bool non_blocking)
        : ip {ip}
        , port {port}
        , receiver {receiver}
        , non_blocking {non_blocking}
    {}

    std::string ip {};
    uint16_t port {};
    bool receiver {};
    bool non_blocking {};
};

class Wrapper_AsioUdp {
private:
    boost::asio::io_service ioService;
    boost::asio::ip::udp::socket socket{ioService};
    boost::asio::ip::udp::endpoint endpoint;
    bool receiver;
    bool non_blocking;

public:
    Wrapper_AsioUdp(const SocketInfo& socketInfo)
        : ioService {}
        , socket {ioService}
        , endpoint {boost::asio::ip::address::from_string(socketInfo.ip), socketInfo.port}
        , receiver {socketInfo.receiver}
        , non_blocking {socketInfo.non_blocking}
    {
        socket.open(boost::asio::ip::udp::v4());
        socket.non_blocking(non_blocking);
        if (receiver) {
            socket.bind(endpoint);
        }
    }

    ~Wrapper_AsioUdp()
    {
        socket.close();
    }

    boost::asio::io_service& io_service()
    {
        return ioService;
    }

    size_t send(const char* const buf, const size_t size)
    {
        size_t bytesSent {};
        if (!receiver) {
            bytesSent = socket.send_to(boost::asio::buffer(buf, size), endpoint);
        }
        return bytesSent;
    }

    size_t recv(char* buf, const size_t size)
    {
        size_t bytesReceived {};
        if (receiver) {
            if (non_blocking) {
                boost::system::error_code error;
                bytesReceived = socket.receive_from(boost::asio::buffer(buf, size), endpoint, 0, error);

                if (error == boost::asio::error::would_block) {
                    //do nothing
                } else if (error) {
                    std::cerr << "Receive error: " << error.message() << std::endl;
                } else {
                    std::cerr << "Received " << bytesReceived << std::endl;
                }
            }
            else {
                bytesReceived = socket.receive_from(boost::asio::buffer(buf, size), endpoint);
            }
        }
        return bytesReceived;
    }
};

class ThreadBase {
public:
    ThreadBase() : running{false} {};
    virtual ~ThreadBase()
    {
        stop();
    }

    inline void start()
    {
        running = true;
        t = std::thread(&ThreadBase::run, this);
    };
    inline void stop() {
        running = false;
        if (t.joinable()) {
            t.join();
        }
    }
protected:
    std::atomic_bool running;
    virtual void run() = 0;
private:
    std::thread t;
};

template<size_t bufferSize>
class TxThread : public ThreadBase
{
public:
    TxThread(const SocketInfo& socketInfo, const std::chrono::milliseconds sleep_time)
        : socketInfo {socketInfo}
        , sleep_time {sleep_time}
    {}

    ~TxThread() override = default;

private:
    void run() override {
        auto udp = Wrapper_AsioUdp(socketInfo);
        while (running) {
            auto bytes = udp.send(buf.data(), buf.size());
            std::this_thread::sleep_for(sleep_time);
        }
    }

    const SocketInfo socketInfo;
    const std::chrono::milliseconds sleep_time;
    std::array<char, bufferSize> buf;
};

template<size_t bufferSize>
class RxThread : public ThreadBase
{
public:
    explicit RxThread(const SocketInfo& socketInfo)
        : socketInfo {socketInfo}
    {}

    ~RxThread() override = default;
private:
    void run() override
    {
        Wrapper_AsioUdp udp{socketInfo};
        while (running) {
            //because the socket is created within this function, cannot stop the socket, or it's io_service
            //  doing it this way REQUIRE that the receiving socket is non-blocking.
            auto bytes = udp.recv(buf.data(), buf.size());
            //"the callback"
            {
                if (bytes > 0) {
                    std::cout << "buffer = " << "0x";
                    for (size_t i = 0; i < buf.size(); i++) {
                        auto byte = static_cast<unsigned char>(buf.at(i));
                        std::cout <<  std::hex << static_cast<int>(byte);
                    }
                    std::cout << std::endl;
                }
            }
            //need a sleep if non-blocking
            if (socketInfo.non_blocking) {
                std::this_thread::sleep_for(std::chrono::microseconds(100));
            }
        }
    }

    const SocketInfo socketInfo;
    std::array<char, bufferSize> buf;
};

int main() {
    const SocketInfo transmitInfo {address, port, false, false};
    const SocketInfo receiveInfo {address, port, true, true};

    TxThread<bufSize> tx(transmitInfo, std::chrono::milliseconds{15});
    RxThread<bufSize> rx(receiveInfo);

    rx.start();
    tx.start();

    std::this_thread::sleep_for(std::chrono::seconds{30});

    tx.stop();
    rx.stop();

	return 0;
}
