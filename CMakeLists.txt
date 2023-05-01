cmake_minimum_required(VERSION 3.22)
project(Sem11 C)

set(CMAKE_C_STANDARD 11)

add_executable(Sem11_Server server.c)
add_executable(Sem11_Client1 client1.c)
add_executable(Sem11_Client2 client2.c)
