cmake_minimum_required(VERSION 3.14)
project(lab3 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Sources (header-only templates don't need .cpp)
set(SOURCES
    src/PrefixSum1D.cpp
    src/PrefixSum2D.cpp
    src/RMQNaive.cpp
    src/FenwickTree.cpp
    src/SparseTable.cpp
    src/HybridRMQ.cpp
    src/main.cpp
)

add_executable(lab3 ${SOURCES})
target_include_directories(lab3 PRIVATE include)
target_compile_options(lab3 PRIVATE -O3 -Wall -Wextra)
