cmake_minimum_required(VERSION 3.12)

#set(PICO_SDK_PATH /home/chandler/External/pico/pico-sdk)
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

include_directories(include
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/lib/pico_rob14450
)

project(main C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

pico_sdk_init()

# Add and link the LCD library
add_library(lcd
    lib/pico_lcd/pico_lcd.cpp
)
target_link_libraries(lcd
    pico_stdlib
    hardware_i2c
)

# Add and link the motor driver library
add_library(motor_driver
    lib/pico_rob14450/pico_rob14450.cpp
)
target_link_libraries(motor_driver
    pico_stdlib
    hardware_pwm
)

# Create the executable for main.cpp
add_executable(${PROJECT_NAME}
    src/main.cpp
)
target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_adc
    lcd
    motor_driver
)
# Generates the binaries for the pico
pico_add_extra_outputs(${PROJECT_NAME})

# Link the stdlib library to the blink file



