cmake_minimum_required(VERSION 3.5)

set(target application)
project(${target} LANGUAGES C)

set(${target}_sources 
	shell/shell_cmds.c 
	os/platform_stub.c 
)

add_library(${target} STATIC ${${target}_sources} )

target_link_libraries(${target}  nr_micro_shell)

# target_include_directories(${target} PUBLIC 

# )
