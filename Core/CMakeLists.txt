cmake_minimum_required(VERSION 3.5)

set(target Core)
project(${target} LANGUAGES C)

set(${target}_sources 
	Src/system_stm32f4xx.c
	Src/main.c 
	Src/gpio.c 
	Src/freertos.c 
	Src/crc.c 
	Src/rng.c 
	Src/usart.c 
	Src/stm32f4xx_it.c 
	Src/stm32f4xx_hal_msp.c 
)

add_library(${target} STATIC ${${target}_sources} )

target_link_libraries(${target}  CMSIS)

include_directories( 
	inc
)
