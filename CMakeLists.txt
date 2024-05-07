cmake_minimum_required(VERSION 3.12)

include(pico_sdk_import.cmake)

project(sparkline VERSION 1.0 LANGUAGES C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

add_executable(sparkline
    pico-ssd1306/ssd1306.c
    sparkline.c
    main.c
    )

target_link_libraries(sparkline
    pico_stdlib
    hardware_adc
    hardware_i2c
    )

target_include_directories(sparkline PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    )

pico_add_extra_outputs(sparkline)
