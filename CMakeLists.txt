cmake_minimum_required(VERSION 4.0)
project(PFT)

set(CMAKE_CXX_STANDARD 20)

add_executable(PFT main.cpp
        Transaction.cpp
        Account.cpp
        Account.h
        Transaction.cpp
)
