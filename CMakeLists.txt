cmake_minimum_required(VERSION 3.23)
project(lueing-common)

set(CMAKE_CXX_STANDARD 17)
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")

find_package(Iconv)

set(SOURCE_FILES lueing_os.cpp lueing_iconv.cpp)
add_library(lueing_common STATIC ${SOURCE_FILES})
target_include_directories(lueing_common PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(lueing_common PUBLIC Iconv::Iconv)

enable_testing()
find_package(GTest CONFIG REQUIRED)
add_executable(lueing_common_test ${SOURCE_FILES} lueing_os_test.cpp lueing_iconv_test.cpp)
target_include_directories(lueing_common_test PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(lueing_common_test PRIVATE Iconv::Iconv GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(lueing_common_test)