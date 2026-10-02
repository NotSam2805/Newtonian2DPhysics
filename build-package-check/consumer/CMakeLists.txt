cmake_minimum_required(VERSION 3.20)
project(N2PConsumer LANGUAGES CXX)

find_package(N2P CONFIG REQUIRED)
add_executable(consumer main.cpp)
target_link_libraries(consumer PRIVATE N2P::N2P)
