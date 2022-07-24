cmake_minimum_required(VERSION 3.5)

set(target FreeRTOS)
project(${target} LANGUAGES C)

set(${target}_sources 
	Source/croutine.c 
	Source/event_groups.c 
	Source/list.c 
	Source/queue.c 
	Source/stream_buffer.c 
	Source/tasks.c 
	Source/timers.c 
	Source/CMSIS_RTOS_V2/cmsis_os2.c 
	Source/portable/MemMang/heap_4.c 
	Source/portable/GCC/ARM_CM4F/port.c
)

add_library(${target} STATIC ${${target}_sources} )

# target_link_libraries(${target}  Core)

include_directories( 
	"Source/include" 
	"Source/CMSIS_RTOS_V2"
	"Source/portable/GCC/ARM_CM4F"
)

