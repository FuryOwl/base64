cmake_minimum_required(VERSION 3.2)

project(mybase64)

set(CMAKE_CXX_STANDARD 14)

add_executable(mybase64 main.cpp getopt_windows.h)

target_include_directories(mybase64 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(UNIX)
        target_compile_definitions(mybase64 PRIVATE UNIX)
        find_library(GETOPT_LIBRARIES getopt)
        target_link_libraries(mybase64 PRIVATE ${CMAKE_LIBRARIES})
elseif(WIN32)
        target_compile_definitions(mybase64 PRIVATE WIN32)
endif()