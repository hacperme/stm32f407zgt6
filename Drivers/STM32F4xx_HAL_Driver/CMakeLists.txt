cmake_minimum_required(VERSION 3.5)

set(target STM32F4xx_HAL_Driver)
project(${target} LANGUAGES C)

set(${target}_sources 
	Src/stm32f4xx_hal_crc.c 
	Src/stm32f4xx_hal_rcc.c 
	Src/stm32f4xx_hal_rcc_ex.c 
	Src/stm32f4xx_hal_flash.c 
	Src/stm32f4xx_hal_flash_ex.c 
	Src/stm32f4xx_hal_flash_ramfunc.c 
	Src/stm32f4xx_hal_gpio.c 
	Src/stm32f4xx_hal_dma_ex.c 
	Src/stm32f4xx_hal_dma.c 
	Src/stm32f4xx_hal_pwr.c 
	Src/stm32f4xx_hal_pwr_ex.c 
	Src/stm32f4xx_hal_cortex.c 
	Src/stm32f4xx_hal.c 
	Src/stm32f4xx_hal_exti.c 
	Src/stm32f4xx_hal_rng.c 
	Src/stm32f4xx_hal_tim.c 
	Src/stm32f4xx_hal_tim_ex.c 
	Src/stm32f4xx_hal_uart.c 
)

add_library(${target} STATIC ${${target}_sources} )

# target_link_libraries(${target}  Core)

include_directories( 
	Inc 
	Inc/Legacy  
)

