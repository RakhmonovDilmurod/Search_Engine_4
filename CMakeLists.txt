cmake_minimum_required (VERSION 3.8)
project("Search_Engine_4")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS ${CMAKE_EXE_LINKER_FLAGS} "-static")

set(MY_INCLUDE_DIR
  ${CMAKE_SOURCE_DIR}/include
)
set(MY_SRC_DIR ${CMAKE_SOURCE_DIR}/src)

include(FetchContent)
FetchContent_Declare(
googletest
URL
https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip)
FetchContent_MakeAvailable(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(gtest_disable_pthreads on)
enable_testing()

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(nlohmann_json)
