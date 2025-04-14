#include <catch2/catch_test_macros.hpp>

#include <cstddef>
#include <iomanip>
#include <string>

#include "test_TransmitReceive.h"

SCENARIO("Transmit and Receive", "[TCP]")
{
    using namespace std::chrono_literals;
    const SocketInfo transmitInfo{"127.0.0.1", 58585, bufSize};
    const SocketInfo receiveInfo{"127.0.0.1", 58585, bufSize};
    RunTest runTest;

    GIVEN("A pair of TCP sockets that are blocking for both the receive and transmit threads")
    {
        auto transmitProperties = determineSocketProperties(SocketType::tcp, false, false);
        auto receiveProperties = determineSocketProperties(SocketType::tcp, true, false);
        auto counts = runTest(transmitInfo, transmitProperties, receiveInfo, receiveProperties);

        // Because the receive thread is stopped first, it may be off by one.
        THEN("The received message count will be the same as the transmitted message count")
        {
            REQUIRE(counts.same_counts());
        }
    }
    GIVEN("A pair of TCP sockets that are nonblocking for both the receive and transmit threads")
    {
        auto transmitProperties = determineSocketProperties(SocketType::tcp, false, true);
        auto receiveProperties = determineSocketProperties(SocketType::tcp, true, true);
        auto counts = runTest(transmitInfo, transmitProperties, receiveInfo, receiveProperties);

        // Non-blocking seems to not do as good.
        THEN("The received message count will be the same as the transmitted message count")
        {
            REQUIRE(counts.almost_same_counts());
        }
    }
    GIVEN("A pair of TCP sockets that are blocking and nonblocking for the receive and transmit threads, respectively")
    {
        auto transmitProperties = determineSocketProperties(SocketType::tcp, false, true);
        auto receiveProperties = determineSocketProperties(SocketType::tcp, true, false);
        auto counts = runTest(transmitInfo, transmitProperties, receiveInfo, receiveProperties);

        // Non-blocking seems to not do as good.
        THEN("The received message count will be the same as the transmitted message count")
        {
            REQUIRE(counts.same_counts());
        }
    }
    GIVEN("A pair of TCP sockets that are nonblocking and blocking for the receive and transmit threads, respectively")
    {
        auto transmitProperties = determineSocketProperties(SocketType::tcp, false, false);
        auto receiveProperties = determineSocketProperties(SocketType::tcp, true, true);
        auto counts = runTest(transmitInfo, transmitProperties, receiveInfo, receiveProperties);

        // Non-blocking seems to not do as good.
        THEN("The received message count will be the same as the transmitted message count")
        {
            REQUIRE(counts.almost_same_counts());
        }
    }
}
