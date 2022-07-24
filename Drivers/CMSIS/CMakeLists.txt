cmake_minimum_required(VERSION 3.5)

set(target CMSIS)
project(${target} LANGUAGES C)

file(WRITE dummy.c " ")

set(${target}_sources 
	dummy.c 
)

add_library(${target} STATIC ${${target}_sources} )

# target_link_libraries(${target}  STM32F4xx_HAL_Driver)

include_directories( 
	Device/ST/STM32F4xx/Include
	Include
)

