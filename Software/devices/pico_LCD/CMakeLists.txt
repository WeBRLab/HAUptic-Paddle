cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_PATH /home/chandler/External/pico/pico-sdk)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)

include_directories(include
    ${CMAKE_CURRENT_LIST_DIR}
)

project(lcd C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDAR 17)

pico_sdk_init()

# Create the executable for blink
add_executable(test_lcd
    test_lcd.cpp
)

add_library(${PROJECT_NAME}
    src/pico_lcd_i2c.cpp
)
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_i2c
)

target_link_libraries(test_lcd
    pico_stdlib
    hardware_i2c
    ${PROJECT_NAME}
)
# Generates the binaries for the pico
pico_add_extra_outputs(test_lcd)

# Link the stdlib library to the blink file



