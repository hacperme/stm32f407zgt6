# STM32F407ZGT6 工程介绍



## 介绍

基于 STM32CubeMX+gcc-arm+openOCD+VSCode 搭建的一个stm32 mcu开发调试环境，主芯片是 stm32f407zgt6。

## 驱动

调试下载使用jlink-ob，需要使用tools目录下的zadig软件替换jlink的驱动。

![[Zadig screenshot]](doc/assets/zadig.png)

## 编译指令

1. 编译，打开cmd 或者 powershell，执行build.bat脚本

   ```powershell
   # 示例
   PS D:\workspace\stm32f407zgt6\stm32f407zgt6> .\build.bat
   NumberOfCores=4, N=8
   D:\workspace\stm32f407zgt6\stm32f407zgt6\tools\gcc_arm_none_eabi\bin/arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -DUSE_HAL_DRIVER -DSTM32F407xx -ICore/Inc -IDrivers/STM32F4xx_HAL_Driver/Inc -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy -IMiddlewares/Third_Party/FreeRTOS/Source/include -IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -IDrivers/CMSIS/Device/ST/STM32F4xx/Include -IDrivers/CMSIS/Include -IMiddlewares/Third_Party/nr_micro_shell/inc -IMiddlewares/Third_Party/cm_backtrace -IMiddlewares/Third_Party/cm_backtrace/Languages/zh-CN -IMiddlewares/Third_Party/cm_backtrace/Languages/en-US  -Og -Wall -fdata-sections -ffunction-sections -Werror -g -gdwarf-2 -MMD -MP -MF"out/main.d" -Wa,-a,-ad,-alms=out/main.lst Core/Src/main.c -o out/main.o
   D:\workspace\stm32f407zgt6\stm32f407zgt6\tools\gcc_arm_none_eabi\bin/arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -DUSE_HAL_DRIVER -DSTM32F407xx -ICore/Inc -IDrivers/STM32F4xx_HAL_Driver/Inc -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy -IMiddlewares/Third_Party/FreeRTOS/Source/include -IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -IDrivers/CMSIS/Device/ST/STM32F4xx/Include -IDrivers/CMSIS/Include -IMiddlewares/Third_Party/nr_micro_shell/inc -IMiddlewares/Third_Party/cm_backtrace -IMiddlewares/Third_Party/cm_backtrace/Languages/zh-CN -IMiddlewares/Third_Party/cm_backtrace/Languages/en-US  -Og -Wall -fdata-sections -ffunction-sections -Werror -g -gdwarf-2 -MMD -MP -MF"out/gpio.d" -Wa,-a,-ad,-alms=out/gpio.lst Core/Src/gpio.c -o out/gpio.o
   D:\workspace\stm32f407zgt6\stm32f407zgt6\tools\gcc_arm_none_eabi\bin/arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -DUSE_HAL_DRIVER -DSTM32F407xx -ICore/Inc -IDrivers/STM32F4xx_HAL_Driver/Inc -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy -IMiddlewares/Third_Party/FreeRTOS/Source/include -IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -IDrivers/CMSIS/Device/ST/STM32F4xx/Include -IDrivers/CMSIS/Include -IMiddlewares/Third_Party/nr_micro_shell/inc -IMiddlewares/Third_Party/cm_backtrace -IMiddlewares/Third_Party/cm_backtrace/Languages/zh-CN -IMiddlewares/Third_Party/cm_backtrace/Languages/en-US  -Og -Wall -fdata-sections -ffunction-sections -Werror -g -gdwarf-2 -MMD -MP -MF"out/freertos.d" -Wa,-a,-ad,-alms=out/freertos.lst Core/Src/freertos.c -o out/freertos.o
   D:\workspace\stm32f407zgt6\stm32f407zgt6\tools\gcc_arm_none_eabi\bin/arm-none-eabi-gcc -c -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -DUSE_HAL_DRIVER -DSTM32F407xx -ICore/Inc -IDrivers/STM32F4xx_HAL_Driver/Inc -IDrivers/STM32F4xx_HAL_Driver/Inc/Legacy -IMiddlewares/Third_Party/FreeRTOS/Source/include -IMiddlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -IMiddlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -IDrivers/CMSIS/Device/ST/STM32F4xx/Include -IDrivers/CMSIS/Include -IMiddlewares/Third_Party/nr_micro_shell/inc -IMiddlewares/Third_Party/cm_backtrace -IMiddlewares/Third_Party/cm_backtrace/Languages/zh-CN -IMiddlewares/Third_Party/cm_backtrace/Languages/en-US  -Og -Wall -fdata-sections -ffunction-sections -Werror -g -gdwarf-2 -MMD -MP -MF"out/crc.d" -Wa,-a,-ad,-alms=out/crc.lst Core/Src/crc.c -o out/crc.o
   
   #...
   
   D:\workspace\stm32f407zgt6\stm32f407zgt6\tools\gcc_arm_none_eabi\bin/arm-none-eabi-gcc out/main.o out/gpio.o out/freertos.o out/crc.o out/rng.o out/usart.o out/stm32f4xx_it.o out/stm32f4xx_hal_msp.o out/stm32f4xx_hal_crc.o out/stm32f4xx_hal_rcc.o out/stm32f4xx_hal_rcc_ex.o out/stm32f4xx_hal_flash.o out/stm32f4xx_hal_flash_ex.o out/stm32f4xx_hal_flash_ramfunc.o out/stm32f4xx_hal_gpio.o out/stm32f4xx_hal_dma_ex.o out/stm32f4xx_hal_dma.o out/stm32f4xx_hal_pwr.o out/stm32f4xx_hal_pwr_ex.o out/stm32f4xx_hal_cortex.o out/stm32f4xx_hal.o out/stm32f4xx_hal_exti.o out/stm32f4xx_hal_rng.o out/stm32f4xx_hal_tim.o out/stm32f4xx_hal_tim_ex.o out/stm32f4xx_hal_uart.o out/system_stm32f4xx.o out/croutine.o out/event_groups.o out/list.o out/queue.o out/stream_buffer.o out/tasks.o out/timers.o out/cmsis_os2.o out/heap_4.o out/port.o out/ansi_port.o out/ansi.o out/nr_micro_shell.o out/shell_cmds.o out/platform_stub.o out/cm_backtrace.o out/startup_stm32f407xx.o out/cmb_fault.o -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard -specs=nano.specs -TSTM32F407ZGTx_FLASH.ld  -lc -lm -lnosys  -Wl,-Map=out/stm32f407zgt6.map,--cref -Wl,--gc-sections -o out/stm32f407zgt6.elf
   D:\workspace\stm32f407zgt6\stm32f407zgt6\tools\gcc_arm_none_eabi\bin/arm-none-eabi-size out/stm32f407zgt6.elf
      text    data     bss     dec     hex filename
     30148     432  107976  138556   21d3c out/stm32f407zgt6.elf
   D:\workspace\stm32f407zgt6\stm32f407zgt6\tools\gcc_arm_none_eabi\bin/arm-none-eabi-objcopy -O ihex out/stm32f407zgt6.elf out/stm32f407zgt6.hex
   D:\workspace\stm32f407zgt6\stm32f407zgt6\tools\gcc_arm_none_eabi\bin/arm-none-eabi-objcopy -O binary -S out/stm32f407zgt6.elf out/stm32f407zgt6.bin
   out\stm32f407zgt6.elf
   已复制         1 个文件。
   out\stm32f407zgt6.hex
   已复制         1 个文件。
   out\stm32f407zgt6.bin
   已复制         1 个文件。
   out\stm32f407zgt6.map
   已复制         1 个文件。
   "### build success !###"
   ```

   编译输出固件在out目录下。

2. 清理

   执行指令 build.bat clean

   ```powershell
   PS D:\workspace\stm32f407zgt6\stm32f407zgt6> .\build.bat clean
   NumberOfCores=4, N=8
   del out /S /Q
   删除文件 - D:\workspace\stm32f407zgt6\stm32f407zgt6\out\ansi.d
   删除文件 - D:\workspace\stm32f407zgt6\stm32f407zgt6\out\ansi.lst
   #...
   ```

3. 烧写

   执行指令 build.bat flash

   ```powershell
   PS D:\workspace\stm32f407zgt6\stm32f407zgt6> .\build.bat flash
   NumberOfCores=4, N=8
   Open On-Chip Debugger 0.11.0 (2021-11-18) [https://github.com/sysprogs/openocd]
   Licensed under GNU GPL v2
   libusb1 09e75e98b4d9ea7909e8837b7a3f00dda4589dc3
   For bug reports, read
           http://openocd.org/doc/doxygen/bugs.html
   Warn : Failed to open device: LIBUSB_ERROR_NOT_SUPPORTED.
   Info : J-Link OB-STM32F072-CortexM compiled Jan  7 2019 14:09:37
   Info : Hardware version: 1.00
   Info : VTarget = 3.300 V
   Info : clock speed 2000 kHz
   Info : SWD DPIDR 0x2ba01477
   Info : stm32f4x.cpu: Cortex-M4 r0p1 processor detected
   Info : stm32f4x.cpu: target has 6 breakpoints, 4 watchpoints
   Info : starting gdb server for stm32f4x.cpu on 3333
   Info : Listening on port 3333 for gdb connections
   target halted due to debug-request, current mode: Thread 
   xPSR: 0x01000000 pc: 0x08004d14 msp: 0x2001a7e8
   Info : Reduced speed from 8000 kHz to 2000 kHz (maximum)
   ** Programming Started **
   Info : device id = 0x10076413
   Info : flash size = 1024 kbytes
   ** Programming Finished **
   ** Resetting Target **
   shutdown command invoked
   ```
   
   
