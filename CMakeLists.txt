cmake_minimum_required(VERSION 3.14)
project(lab3 CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB_SOURCES
    src/PrefixSum1D.cpp
    src/PrefixSum2D.cpp
    src/RMQNaive.cpp
    src/FenwickTree.cpp
    src/SparseTable.cpp
    src/HybridRMQ.cpp
)

# Main benchmark
add_executable(lab3 ${LIB_SOURCES} src/main.cpp)
target_include_directories(lab3 PRIVATE include)
target_compile_options(lab3 PRIVATE -O3 -Wall -Wextra)

# Tests (Catch2)
find_package(Catch2 3 REQUIRED)

add_executable(lab3_tests ${LIB_SOURCES} tests/test_all.cpp)
target_include_directories(lab3_tests PRIVATE include)
target_compile_options(lab3_tests PRIVATE -O2 -Wall -Wextra)
target_link_libraries(lab3_tests PRIVATE Catch2::Catch2WithMain)

include(CTest)
include(Catch)
catch_discover_tests(lab3_tests)
