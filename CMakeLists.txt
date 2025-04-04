cmake_minimum_required(VERSION 3.30)
project(boost_socket_testing)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.74.0 REQUIRED COMPONENTS system)  # Add needed components

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found!")
endif()

add_executable(boost_socket_testing
    main.cpp
    Thread/ThreadBase.cpp
    Thread/ThreadBase.h
    Thread/RxThread.cpp
    Thread/RxThread.h
    Thread/TxThread.cpp
    Thread/TxThread.h
    Thread/SocketInfo.h
    Thread/Wrapper_AsioUdp.cpp
    Thread/Wrapper_AsioUdp.h
    Thread/SocketThread.cpp
    Thread/SocketThread.h
)

target_link_libraries(boost_socket_testing PRIVATE
    Boost::system
    ws2_32
)