cmake_minimum_required(VERSION 3.5)

set(target stm32_test_project)
project(${target} LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

set(OBJS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/output)
set (LIBRARY_OUTPUT_PATH ${OBJS_PATH}/libs)
set (EXECUTABLE_OUTPUT_PATH ${OBJS_PATH}/bin)

set(ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")

include(${ROOT_DIR}/scripts/project_stm32f407zgt6.cmake)

file(WRITE dummy.c "")
add_executable(${target}.elf dummy.c)
