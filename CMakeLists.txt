cmake_minimum_required(VERSION 3.19)
project(SymbDiffProject)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC "src/*.cpp")

add_library(symdiff STATIC ${SRC})

include_directories(include)

add_executable(differentiator main.cpp)
target_link_libraries(differentiator symdiff)

add_executable(tests test/test.cpp)
target_link_libraries(tests symdiff)