cmake_minimum_required(VERSION 3.27)
project(log-lib)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON) 

include_directories(include)

file(GLOB_RECURSE SRC_FILES src/*.cpp)

add_executable(example ${SRC_FILES})

target_compile_options(example PRIVATE -O2 -Wall -Wextra -pedantic -pedantic-errors)

# include(FetchContent)
# FetchContent_Declare(
#   googletest
#   URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
# )
# FetchContent_MakeAvailable(googletest)

# enable_testing()

# add_executable(
#   runUnutTests
#   test/test.cpp
#   src/graph.cpp
# )
# target_link_libraries(
#   runUnutTests
#   GTest::gtest_main
# )

# include(GoogleTest)
# gtest_discover_tests(runUnutTests)