cmake_minimum_required(VERSION 3.30)
set(target "unit-test")

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.74.0 REQUIRED COMPONENTS system)  # Add needed components

if(NOT Boost_FOUND)
    message(FATAL_ERROR "Boost not found!")
endif()

include(FetchContent)
FetchContent_Declare(
    catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.5.3
)
FetchContent_MakeAvailable(catch2)

set(base_dir "${CMAKE_SOURCE_DIR}")
message("cwd = ${base_dir}")
add_executable(${target}
    test_main.cpp
    ${base_dir}/Thread/ThreadBase.cpp
    ${base_dir}/Thread/ReceiveSocketThread.cpp
    ${base_dir}/Thread/TransmitSocketThread.cpp
    ${base_dir}/Socket/Wrapper_AsioUdp.cpp
    ${base_dir}/Thread/SocketThread.cpp
    ${base_dir}/Socket/Wrapper_AsioTcp.cpp
    ${base_dir}/Socket/Base_AsioSocket.cpp
    test_TCP_TransmitReceive.cpp
    test_UDP_TransmitReceive.cpp
)

target_include_directories(${target} PUBLIC
    ${base_dir}
    ${base_dir}/Socket
    ${base_dir}/Thread
    ${base_dir}/Utility
)

# Enable testing
enable_testing()

target_link_libraries(${target} PRIVATE
    Boost::system
    Catch2::Catch2
    ws2_32
)

include(CTest)
include(Catch)