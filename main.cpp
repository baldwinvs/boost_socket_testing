#include <thread>
#include <boost/asio.hpp>

#include <array>
#include <atomic>
#include <cstdio>
#include <cstddef>
#include <cstring>
#include <iomanip>
#include <iostream>

constexpr uint16_t portToUse {141};
constexpr size_t bufSize {16};
inline const std::string address {"127.0.0.1"};
std::atomic_bool received {true};

int receiver() {
    std::array<char, bufSize> buf {};

    boost::asio::io_service ioService;
    boost::asio::ip::udp::socket socket{ioService};
    boost::asio::ip::udp::endpoint endpoint {boost::asio::ip::address::from_string(address), portToUse};

    socket.open(boost::asio::ip::udp::v4());
    socket.bind(endpoint);

    while(1) {
        auto len = socket.receive_from(boost::asio::buffer(buf.data(), sizeof(buf)), endpoint);
        //print the data
        std::cout.write(buf.data(), len);
        std::cout << std::endl;
        received = true;
    }
    return 0;
}

int transmitter() {
    std::array<char, bufSize> buf {};
    char fillChar {'0'};

    boost::asio::io_service ioService;
    boost::asio::ip::udp::socket socket{ioService};

    socket.open(boost::asio::ip::udp::v4());

    boost::asio::ip::udp::endpoint endpoint {boost::asio::ip::address::from_string(address), portToUse};

    using namespace std::chrono_literals;
    while(1) {
        if(received) {
            received = false;
            memset(buf.data(), fillChar, buf.size());

            socket.send_to(boost::asio::buffer(buf), endpoint);
            if(++fillChar > '9')
                fillChar = '0';
        }
        std::this_thread::sleep_for(100ms);
    }
    return 0;
}

int main() {
    std::thread(receiver).detach();
    std::thread(transmitter).detach();

    while(1) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(60s);
    }
	return 0;
}
