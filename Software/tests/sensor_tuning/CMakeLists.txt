cmake_minimum_required(VERSION 3.12)

#set(PICO_SDK_PATH /home/chandler/External/pico/pico-sdk)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

include_directories(include
    ${CMAKE_CURRENT_LIST_DIR}
)

project(ui_pot C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDAR 17)

pico_sdk_init()

# Create the executable for blink
add_executable(${PROJECT_NAME}
    src/ui_pot.cpp
)

add_library(pico_lcd
    src/pico_lcd_i2c.cpp
)

target_link_libraries(pico_lcd
    pico_stdlib
    hardware_i2c
)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_i2c
    hardware_adc
    pico_lcd

)
# Generates the binaries for the pico
pico_add_extra_outputs(${PROJECT_NAME})




