cmake_minimum_required(VERSION 3.16)

# Project name and language
project(Module1_Fundamentals C ASM)

# Define the executable
add_executable(firmware
    main.c
    startup.c
)

# Linker script
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/linker_script.ld)
target_link_options(firmware PRIVATE -T${LINKER_SCRIPT} -Wl,-Map=${CMAKE_PROJECT_NAME}.map)

# Create a raw binary file from the ELF (for flashing/simulation)
add_custom_command(TARGET firmware POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary firmware firmware.bin
    COMMENT "Generating binary file firmware.bin"
)

# Print size information
add_custom_command(TARGET firmware POST_BUILD
    COMMAND ${CMAKE_SIZE} firmware
    COMMENT "Printing size information"
)
