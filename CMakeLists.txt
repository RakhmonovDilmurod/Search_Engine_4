cmake_minimum_required(VERSION 3.5)

project(search_engine_4 LANGUAGES CXX)

include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip

    )
include(GoogleTest)
FetchContent_MakeAvailable(googletest)
set(Sources main.cpp)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(JSON_BuildTests OFF CACHE INTERNAL "")
set(DOWNLOAD_EXTRACT_TIMESTAMP true)
set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(MY_SRC_DIR ${CMAKE_SOURCE_DIR}/src)
add_subdirectory(src)
add_subdirectory(tests)


include_directories(search_engine_4 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/nlohmann_json/include)
target_link_libraries(search_engine_4 PRIVATE gtest_main)
gtest_discover_tests(search_engine_4)
enable_testing()

install(TARGETS search_engine_4
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES config/answers.json config/requests.json config/config.json
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/search_engine_4/config)
