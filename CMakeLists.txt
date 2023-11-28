# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_BUILD_TYPE "Release") 

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
# set(PICO_SDK_PATH "/home/jabba/pico-sdk")

set(PICO_BOARD pico CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

project(pico-wspr-tx C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(pico-wspr-tx)

pico_generate_pio_header(pico-wspr-tx ${CMAKE_CURRENT_LIST_DIR}/pico-hf-oscillator/piodco/dco.pio)

target_sources(pico-wspr-tx PUBLIC
	             ${CMAKE_CURRENT_LIST_DIR}/pico-hf-oscillator/lib/assert.c
               ${CMAKE_CURRENT_LIST_DIR}/pico-hf-oscillator/piodco/piodco.c
               ${CMAKE_CURRENT_LIST_DIR}/pico-hf-oscillator/gpstime/GPStime.c
               ${CMAKE_CURRENT_LIST_DIR}/TxChannel/TxChannel.c
               ${CMAKE_CURRENT_LIST_DIR}/WSPRbeacon/thirdparty/WSPRutility.c
               ${CMAKE_CURRENT_LIST_DIR}/WSPRbeacon/thirdparty/nhash.c
               ${CMAKE_CURRENT_LIST_DIR}/WSPRbeacon/WSPRbeacon.c
               ${CMAKE_CURRENT_LIST_DIR}/debug/logutils.c
               ${CMAKE_CURRENT_LIST_DIR}/init.c
               ${CMAKE_CURRENT_LIST_DIR}/core1.c
               ${CMAKE_CURRENT_LIST_DIR}/main.c
              )

pico_set_program_name(pico-wspr-tx "pico-wspr-tx")
pico_set_program_version(pico-wspr-tx "0.1")

pico_enable_stdio_uart(pico-wspr-tx 1)
pico_enable_stdio_usb(pico-wspr-tx 1)

# Add the standard include files to the build
target_include_directories(pico-wspr-tx PRIVATE
                           ${CMAKE_CURRENT_LIST_DIR}
                           ${CMAKE_CURRENT_LIST_DIR}/pico-hf-oscillator/piodco
                           ${CMAKE_CURRENT_LIST_DIR}/pico-hf-oscillator/debug
                           ${CMAKE_CURRENT_LIST_DIR}/TxChannel
                           ${CMAKE_CURRENT_LIST_DIR}/WSPRbeacon
                           ${CMAKE_CURRENT_LIST_DIR}/WSPRbeacon/thirdparty
                           ${CMAKE_CURRENT_LIST_DIR}/..
                          )

# Add any user requested libraries
target_link_libraries(
        pico-wspr-tx
        pico_stdlib
        pico_sync
        pico_multicore
        hardware_timer
        hardware_clocks
        hardware_pio
        hardware_vreg
        )

pico_add_extra_outputs(pico-wspr-tx)
