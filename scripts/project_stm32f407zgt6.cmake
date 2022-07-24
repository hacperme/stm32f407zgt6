cmake_minimum_required(VERSION 3.10)

if(CMAKE_BUILD_TYPE MATCHES Debug)
set(DBG_FLAGS "-g -gdwarf-2 -Og")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
# set(DBG_FLAGS "-Og")
set(DBG_FLAGS "-g -gdwarf-2 -Og")
endif()

set(MCU_ARCH ARMCM4)
set(MCU_FAMILY STM32F407xx)
set(MCU_NAME STM32F407ZGTx)


set(LINKER_SCRIPT ${ROOT_DIR}/${MCU_NAME}_FLASH.ld)


set(CPU "-mcpu=cortex-m4")
set(FPU "-mfpu=fpv4-sp-d16")
set(FLOAT_ABI "-mfloat-abi=hard")


## auto-set variables from user input
set(MCU_FLAGS "${CPU} -mthumb ${FPU} ${FLOAT_ABI}")
set(LINK_FLAGS "${MCU_FLAGS} -Wl,--gc-sections")
set(EXTRA_LINK_FLAGS "-Wl,-Map=${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.map,--cref -specs=nano.specs -specs=nosys.specs")


# message(STATUS "MCU_FLAGS: ${MCU_FLAGS}")

# compiler: language specific flags
set(CMAKE_C_FLAGS "${MCU_FLAGS} -Wall -fdata-sections -ffunction-sections -Werror ${DBG_FLAGS} " CACHE INTERNAL "C compiler flags")
set(CMAKE_C_FLAGS_DEBUG "" CACHE INTERNAL "C compiler flags: Debug")
set(CMAKE_C_FLAGS_RELEASE "" CACHE INTERNAL "C compiler flags: Release")

set(CMAKE_CXX_FLAGS "${MCU_FLAGS} -fno-rtti -fno-exceptions -fno-builtin -Wall -fdata-sections -ffunction-sections -Werror ${DBG_FLAGS} " CACHE INTERNAL "Cxx compiler flags")
set(CMAKE_CXX_FLAGS_DEBUG "" CACHE INTERNAL "Cxx compiler flags: Debug")
set(CMAKE_CXX_FLAGS_RELEASE "" CACHE INTERNAL "Cxx compiler flags: Release")

set(CMAKE_ASM_FLAGS "${MCU_FLAGS} -x assembler-with-cpp ${DBG_FLAGS} " CACHE INTERNAL "ASM compiler flags")
set(CMAKE_ASM_FLAGS_DEBUG "" CACHE INTERNAL "ASM compiler flags: Debug")
set(CMAKE_ASM_FLAGS_RELEASE "" CACHE INTERNAL "ASM compiler flags: Release")

set(CMAKE_EXE_LINKER_FLAGS "${LINK_FLAGS} -T${LINKER_SCRIPT} ${EXTRA_LINK_FLAGS}" CACHE INTERNAL "Exe linker flags")

add_definitions(
    -DUSE_HAL_DRIVER
    -DSTM32F407xx
)

# name of targets
set(BIN_TARGET ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.bin)
set(HEX_TARGET ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.hex)
set(MAP_TARGET ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.map)
set(LSS_TARGET ${EXECUTABLE_OUTPUT_PATH}/${PROJECT_NAME}.lss)



