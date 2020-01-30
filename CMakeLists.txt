cmake_minimum_required(VERSION 3.15)
project(SMH)

set(PLATFORM CP)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp lib/WString.cpp lib/avr/dtostrf.c lib/itoa.c lib/Print.cpp src/SMH.cpp src/USART.cpp src/Commands.cpp)

add_executable(SMH ${SOURCE_FILES})
