/* 
 * Lab 1.1: The Linker Script
 * Defines the memory layout for our "Virtual" ARM Cortex-M3
 */

/* Entry Point: The first instruction to execute */
ENTRY(Reset_Handler)

/* Memory Definitions */
/* We assume a generic Cortex-M3 layout often used in QEMU (MPS2-AN385 or similar) */
/* Flash: Starts at 0x00000000, Size 128KB */
/* RAM:   Starts at 0x20000000, Size 64KB */
MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 128K
    RAM   (rwx): ORIGIN = 0x20000000, LENGTH = 64K
}

/* Sections Definitions */
SECTIONS
{
    /* The Vector Table and Main Code go into FLASH */
    .text :
    {
        . = ALIGN(4);
        KEEP(*(.isr_vector)) /* Keep the interrupt vector table */
        *(.text*)            /* All .text sections (code) */
        *(.rodata*)          /* Read-only data (constants) */
        . = ALIGN(4);
        _etext = .;          /* Symbol: End of text section (used by startup code) */
    } > FLASH

    /* Initialized Data: Stored in FLASH, copied to RAM at startup */
    .data : AT (_etext)
    {
        . = ALIGN(4);
        _sdata = .;          /* Symbol: Start of data in RAM */
        *(.data*)
        . = ALIGN(4);
        _edata = .;          /* Symbol: End of data in RAM */
    } > RAM

    /* Uninitialized Data: Zeroed out in RAM at startup */
    .bss :
    {
        . = ALIGN(4);
        _sbss = .;           /* Symbol: Start of bss */
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;           /* Symbol: End of bss */
    } > RAM

    /* Stack Pointer: Top of RAM */
    _estack = ORIGIN(RAM) + LENGTH(RAM);
}
