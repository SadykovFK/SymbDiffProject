cmake_minimum_required(VERSION 3.19)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Expression)

file(GLOB SRC src/*.cpp)
add_library(src STATIC ${SRC})

include_directories(include)

# Добавляем Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

# Исполняемый файл с тестами
add_executable(tests test/test.cpp)
target_link_libraries(tests PRIVATE src gtest gtest_main)
add_test(NAME tests COMMAND tests)

# Главный исполняемый файл дифференциатора
add_executable(differentiator main.cpp)
target_link_libraries(differentiator PRIVATE src)
