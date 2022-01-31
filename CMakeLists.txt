cmake_minimum_required(VERSION 3.21)
project(mqlib)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(${PROJECT_SOURCE_DIR}/def)

add_library(mqlib mq.hpp def/instance.hpp impl/instance.cpp def/handler.hpp impl/handler.cpp)

target_compile_options(mqlib PRIVATE -DDEBUG)
target_link_libraries(mqlib uv z pthread ssl crypto)
