cmake_minimum_required(VERSION 3.0)


set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER arm-g++)
set(CMAKE_C_COMPILER arm-gcc)

add_compile_options(
	-std=gnu++11
	-Wall
	-Werror
	-O2
)

set(LIB_NAME pwm)
add_library(${LIB_NAME} STATIC src/pwm.cpp)
target_include_directories(${LIB_NAME} PUBLIC include)
install(TARGETS ${LIB_NAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)