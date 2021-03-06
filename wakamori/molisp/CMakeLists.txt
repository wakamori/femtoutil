# requred cmake version
cmake_minimum_required (VERSION 2.6)

# build type
set (CMAKE_BUILD_TYPE Debug)

# compile options (Debug)
set (CMAKE_C_FLAGS_DEBUG "-W -Wall -g3 -O0")

# compile options (Release)
set (CMAKE_C_FLAGS_RELEASE "-W -Wall -g0 -O2")

# project name
project (Molisp)

# define macro
set (ML_VERSION_MAJOR 1)
set (ML_VERSION_MINOR 0)

# define MODE_DEBUG macro
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(MODE_DEBUG 1)
endif()

include_directories (
	"include"
)

configure_file (
	"src/ml_config.h.in"
	"include/ml_config.h"
)

add_executable (
	build/molisp
	src/ml_eval.c
	src/ml_main.c
	src/ml_memory.c
	src/ml_print.c
	src/ml_read.c
	src/molisp.c
)

target_link_libraries (
	build/molisp
	readline
)
