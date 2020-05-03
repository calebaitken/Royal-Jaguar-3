cmake_minimum_required(VERSION 3.14)
project(Test_Client)

add_compile_definitions(_WIN32_WININT)

set(CMAKE_CXX_STANDARD 11)

add_executable(Test_Client src/main.cpp)

file(COPY libs/ DESTINATION "${CMAKE_BINARY_DIR}")

target_link_libraries(Test_Client -lWs2_32)