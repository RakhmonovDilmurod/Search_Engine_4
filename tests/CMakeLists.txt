cmake_minimum_required(VERSION 3.2)

set(MY_SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)

# Add GoogleTest fetch
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)

# Add GoogleTest directory
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
# GoogleTest: For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
# Switch on GoogleTest fetch
FetchContent_MakeAvailable(googletest)
# Switch on testing
enable_testing()
# Googletest to CMake fix (if need)
set (gtest_disable_pthreads on)

set(SrcFiles
	${MY_SRC_DIR}/InvertedIndex.cpp
	${MY_SRC_DIR}/SearchEngine.cpp
	${MY_SRC_DIR}/ConverterJSON.cpp
)

set(TestsFiles
	InvertedIndex_test.cpp
	SearchEngine_test.cpp
	ConverterJSON_test.cpp
)

# Test configuration
add_executable(GtestTests ${SrcFiles} ${TestsFiles})

include_directories(GtestTests PUBLIC ${MY_INCLUDE_DIR})
# Add GoogleTest directory
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# Googletest library
target_link_libraries(GtestTests PRIVATE gtest_main nlohmann_json::nlohmann_json gtest_main)
include(GoogleTest)

