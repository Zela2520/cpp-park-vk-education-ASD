cmake_minimum_required(VERSION 3.13)
project(HW)

set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "-g -O0 -Wall -Wextra -Werror -Wpedantic -pedantic -Wconversion")

add_executable(1_1
        module1/bitCheck.cpp)

add_executable(1_2
       module1/twinSearch.cpp)

add_executable(1_3
       module1/queueWithStack.cpp)

add_executable(1_4 module1/mergeArraysByHeap.cpp)

add_executable(1_5 module1/advertsing.cpp)

add_executable(1_6 module1/kStatistic.cpp)
# add_executable(1_6 module1/kStatisticDone.cpp)

add_executable(1_7 module1/msd.cpp)


add_executable(2_1 module2/hashTable.cpp)

add_executable(2_1_2 module2/chainHashTable.cpp)

add_executable(2_2 module2/postOrder.cpp)

# add_executable(2_3 module2/BTree.cpp)

add_executable(2_4 module2/soldier.cpp)
