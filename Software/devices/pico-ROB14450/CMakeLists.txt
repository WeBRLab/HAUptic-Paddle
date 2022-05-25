cmake_minimum_required(VERSION 3.12)

set(PICO_SDK_PATH /home/chandler/External/pico/pico-sdk)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include_directories(include
${CMAKE_CURRENT_LIST_DIR}
)

project(motor C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# Create the executable for blink
add_executable(${PROJECT_NAME}
    src/motor.cpp
)
add_library(ROB14450
    src/ROB14450.cpp    
)
target_link_libraries(ROB14450
    pico_stdlib
    hardware_pwm
)

# Generates the binaries for the pico
pico_add_extra_outputs(${PROJECT_NAME})

# Link the stdlib library to the blink file
target_link_libraries(${PROJECT_NAME}
    ROB14450
)


