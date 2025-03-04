cmake_minimum_required(VERSION 3.20)

project(SymDiff)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(SymDiffFiles)
add_subdirectory(SymDiffFiles)

add_executable(test test.cpp)
target_link_libraries(test SymDiff)