cmake_minimum_required(VERSION 3.16)

project(Module4_CAN C ASM)

# Add executable
add_executable(firmware
    main.c
    startup.c
    can_driver.c
    ../../module_2_uart/project/uart_driver.c
)

# Include directories
target_include_directories(firmware PRIVATE
    .
    ../../module_2_uart/project
)

# Reuse the linker script from Module 1
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/../../module_1_fundamentals/project/linker_script.ld)
target_link_options(firmware PRIVATE -T${LINKER_SCRIPT} -Wl,-Map=${CMAKE_PROJECT_NAME}.map)

# Binary generation
add_custom_command(TARGET firmware POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary firmware firmware.bin
    COMMENT "Generating binary file firmware.bin"
)
