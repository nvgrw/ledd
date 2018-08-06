cmake_minimum_required (VERSION 2.6)
project (ledd)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Debug)
endif (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -D_THREAD_SAFE -std=gnu99 -Werror -pedantic")

file (GLOB SOURCES "src/*.c" "include/*.h")

find_library (WS281LIB ws2811 HINTS deps/rpi_ws281x)

add_executable (ledd "${SOURCES}")
include_directories(include deps)

target_link_libraries (ledd "${WS281LIB}")
