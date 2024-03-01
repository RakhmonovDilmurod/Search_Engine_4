cmake_minimum_required (VERSION 3.8)
project("Search_Engine_4")

set(CMAKE_CXX_STANDARD 20)

set(MY_INCLUDE_DIR
  ${CMAKE_SOURCE_DIR}/include
)
set(MY_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
include_directories(${MY_INCLUDE_DIR})
include(FetchContent)

add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(nlohmann_json)
