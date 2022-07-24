cmake_minimum_required(VERSION 3.5)

set(target stm32f407zgt6)
project(${target} LANGUAGES C CXX ASM)
set(ELF_TARGET ${PROJECT_NAME}.elf)

set(OBJS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/output)
set (LIBRARY_OUTPUT_PATH ${OBJS_PATH}/libs)
set (EXECUTABLE_OUTPUT_PATH ${OBJS_PATH}/bin)

set(ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}")

file(WRITE dummy.c " ")
add_executable(${ELF_TARGET}  
    dummy.c 
    startup_stm32f407xx.s
)

include(${ROOT_DIR}/scripts/project_stm32f407zgt6.cmake)

include_directories( 
	Core/Inc
    Drivers/STM32F4xx_HAL_Driver/Inc
    Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
    Middlewares/Third_Party/FreeRTOS/Source/include
    Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
    Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
    Drivers/CMSIS/Device/ST/STM32F4xx/Include
    Drivers/CMSIS/Include
    Middlewares/Third_Party/nr_micro_shell/inc
)

target_link_libraries(${ELF_TARGET} PUBLIC 
    -lc 
    -lm 
    -lnosys 
    CMSIS
    Core
    nr_micro_shell 
    application
    FreeRTOS 
    STM32F4xx_HAL_Driver 
)

# create binary & hex files and show size of resulting firmware image
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_TARGET}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_TARGET}
        COMMAND ${CMAKE_OBJDUMP} -S $<TARGET_FILE:${PROJECT_NAME}.elf> > ${LSS_TARGET}
        COMMAND ${ARM_SIZE_EXECUTABLE} -B ${EXECUTABLE_OUTPUT_PATH}/${ELF_TARGET} ${HEX_TARGET}
        COMMENT "Generating ${HEX_TARGET}, ${BIN_TARGET}")



add_custom_target(flash
    DEPENDS ${ELF_TARGET} 
    COMMAND ${OPENOCD_EXECUTABLE} -f ${ROOT_DIR}/openocd.cfg -c "program ${EXECUTABLE_OUTPUT_PATH}/${ELF_TARGET} reset exit"
)

add_subdirectory(Core)
add_subdirectory(Drivers)
add_subdirectory(Middlewares)
add_subdirectory(application)

