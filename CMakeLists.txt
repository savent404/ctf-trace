cmake_minimum_required(VERSION 3.20)
project(HelloWorld CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# fetch the google test framework
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


enable_testing()
add_executable(ut ut_cft_basic.cpp)
target_link_libraries(ut GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(ut)
