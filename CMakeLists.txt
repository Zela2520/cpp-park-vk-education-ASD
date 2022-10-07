cmake_minimum_required(VERSION 3.13)
project(HW)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra -Werror -Wpedantic")

# add_executable(1_1
#         module1/bitCheck.cpp)

add_executable(1_3
        module1/queueWithStack.cpp)
