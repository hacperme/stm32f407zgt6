cmake_minimum_required(VERSION 3.5)

set(target nr_micro_shell)
project(${target} LANGUAGES C)

set(${target}_sources 
	src/ansi_port.c 
	src/ansi.c 
	src/nr_micro_shell.c 
)

add_library(${target} STATIC ${${target}_sources} )

# target_link_libraries(${target}  )


