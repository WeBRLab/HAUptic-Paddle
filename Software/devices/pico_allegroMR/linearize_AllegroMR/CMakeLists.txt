cmake_minimum_required(VERSION 3.12)
set(PICO_SDK_PATH /home/avinash/rpi_pico/pico/pico-sdk)
include(${PICO_SDK_PATH}/external/pico_sdk_import.cmake)
include_directories(include
    ${CMAKE_CURRENT_LIST_DIR}
)

project(linearize_AllegroMR C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDAR 17)
pico_sdk_init()

# Create executable for CMake
add_executable(linearizeMR
)

target_link_libraries(${PROJECT_NAME}
    pico_stdlib
    hardware_adc
)

target_link_libraries(linearizeMR
    pico_stdlib
    hardware_adc
    ${PROJECT_NAME}
)
# Generates the binaries for the pico
pico_add_extra_outputs(linearizeMR)


