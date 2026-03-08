cmake_minimum_required(VERSION 3.10)
project(Graph C)
set(CMAKE_C_STANDART 11)
set(CMAKE_C_STANDART_REQUIRED TRUE)
include_directories(/usr/include/readline)
find_library(READLINE_LIBRARY
	NAMES readline
	PATH /usr/lib
)
if (READLINE_LIBRARY)
	message(STATUS "Readline found")
	set(SOURCE_FILES src/main.c src/graph.c src/queue_l.c)
	add_executable(Graph ${SOURCE_FILES})
	target_include_directories(Graph PRIVATE headers)
	target_link_libraries(Graph ${READLINE_LIBRARY})
	set_target_properties(Graph PROPERTIES COMPILE_FLAGS "-Wall -Wextra")
else()
	message(FATAL_ERROR "Readline not found")
endif()
