cmake_minimum_required(VERSION 3.0)

find_package(Threads REQUIRED)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_CXX_COMPILER arm-g++)
set(CMAKE_C_COMPILER arm-gcc)

add_compile_options(
	-std=gnu++11
	-Wall
	-Werror
	-O2
)

include_directories(../include)
set(SOURCE test.cpp ../src/pwm.cpp)
add_executable(testbench ${SOURCE})
install(TARGETS testbench DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/bin)