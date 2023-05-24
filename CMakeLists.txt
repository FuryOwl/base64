cmake_minimum_required(VERSION 3.2)

project(base64)

set(CMAKE_CXX_STANDARD 14)

add_executable(base64 main.cpp getopt_windows.h)

target_include_directories(base64 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(UNIX)
	target_compile_definitions(base64 PRIVATE UNIX)
esleif(WIN32)
	target_compile_definitions(base64 PRIVATE WIN32)
endif()