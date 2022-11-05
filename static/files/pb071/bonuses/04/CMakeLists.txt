cmake_minimum_required(VERSION 3.0)

# Project configuration
project(seminar04-bonus-maze)
set(SOURCES maze.h maze.c)
set(EXECUTABLE maze)

# Executable
add_executable(maze ${SOURCES} main.c)
add_executable(test_maze ${SOURCES} cut.h test_maze.c)

# Configure compiler warnings
if (CMAKE_C_COMPILER_ID MATCHES Clang OR ${CMAKE_C_COMPILER_ID} STREQUAL GNU)
  # using regular Clang, AppleClang or GCC
  # Strongly suggested: neable -Werror
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -Wall -Wextra -pedantic")
elseif (${CMAKE_C_COMPILER_ID} STREQUAL MSVC)
  # using Visual Studio C++
  target_compile_definitions(${EXECUTABLE} PRIVATE _CRT_SECURE_NO_DEPRECATE)
  set(CMAKE_CXX_FLAGS "/permissive- /W4 /EHsc")
endif()

if(MINGW)
    target_compile_definitions(${EXECUTABLE} PRIVATE __USE_MINGW_ANSI_STDIO=1)
endif()
