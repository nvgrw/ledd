cmake_minimum_required (VERSION 2.6)
project (ledd)

if (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)
  set (CMAKE_BUILD_TYPE Debug)
endif (NOT CMAKE_CONFIGURATION_TYPES AND NOT CMAKE_BUILD_TYPE)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -D_THREAD_SAFE -std=c99 -Werror -pedantic")

file (GLOB SOURCES "src/*.c" "include/*.h")
include_directories (include)

add_executable (ledd "${SOURCES}")
