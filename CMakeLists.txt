cmake_minimum_required(VERSION 4.0)
project(PFT)

set(CMAKE_CXX_STANDARD 20)
include_directories(include)
include_directories(/opt/homebrew/opt/boost/include)
add_executable(PFT main.cpp
        Transaction.cpp
        Account.cpp
        Transaction.cpp
)
