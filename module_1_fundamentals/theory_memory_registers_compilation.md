# Module 1: The Silicon & The Software - Theory Deep Dive

## 1. The Compilation Process: From C to Machine Code

When you click "Build", a complex chain of events occurs to transform your human-readable C code into the binary instructions the processor understands.

### 1.1 The Four Stages
1.  **Preprocessing** (`.c` -> `.i`):
    *   Handles directives starting with `#` (`#include`, `#define`).
    *   Replaces macros, includes file contents.
    *   *Try it*: `arm-none-eabi-gcc -E main.c -o main.i`

2.  **Compilation** (`.i` -> `.s`):
    *   Translates C code into **Assembly Language** specific to the target architecture (ARM Thumb-2).
    *   Optimizations happen here (-O0, -O2, -Os).
    *   *Try it*: `arm-none-eabi-gcc -S main.c -o main.s`

3.  **Assembly** (`.s` -> `.o`):
    *   Translates Assembly mnemonics (e.g., `MOV R0, #1`) into machine code (binary opcodes).
    *   Produces a **Relocatable Object File**. It contains code but doesn't know *where* in memory it will live yet.

4.  **Linking** (`.o` + `.o` -> `.elf`):
    *   Combines all object files and libraries.
    *   Resolves symbols (e.g., if `main.c` calls `UART_Init()` in `uart.c`, the linker connects them).
    *   **Crucial Step**: Assigns absolute memory addresses to code and data based on the **Linker Script** (`.ld`).

## 2. Memory Architecture

Embedded systems typically have two main types of memory:
1.  **Flash (ROM)**: Non-volatile. Stores the code (instructions) and constant data (read-only).
2.  **SRAM (RAM)**: Volatile. Stores variables, the stack, and the heap.

### 2.1 The Sections
The linker groups data into "sections":
*   `.text`: The code instructions. (Lives in Flash)
*   `.rodata`: Read-only constants (e.g., `const int x = 5;`). (Lives in Flash)
*   `.data`: Initialized global/static variables (e.g., `int count = 10;`).
    *   *Tricky part*: These values live in Flash (so they persist after power off) but must be copied to RAM at startup so they can be modified.
*   `.bss`: Uninitialized global/static variables (e.g., `int buffer[1024];`).
    *   Must be zeroed out at startup.
*   `.stack`: The memory used for local variables and function calls.

## 3. Registers & Memory-Mapped I/O (MMIO)

How does software control hardware (like turning on an LED)?
In ARM systems, peripherals (GPIO, UART, Timers) are mapped to specific memory addresses.

*   **Memory Map**: A giant table telling you which address corresponds to which peripheral.
    *   Example: Address `0x40021018` might be the "Output Data Register" for GPIO Port A.
*   **Dereferencing**: To write to a register, we treat the address as a pointer.
    ```c
    #define GPIOA_ODR  (*((volatile uint32_t *)0x40021018))
    GPIOA_ODR = 0x1; // Writes 1 to the register
    ```
*   **The `volatile` Keyword**: Tells the compiler "Do not optimize this access".
    *   Without `volatile`, the compiler might think "You wrote to this address, but never read it back, so I'll delete that instruction to save speed."
    *   For hardware registers, every read/write has a side effect (e.g., clearing an interrupt flag), so we *must* force the compiler to do exactly what we say.

## 4. Bitwise Operations

Registers are often 32-bits wide, but we only want to change *one* bit (e.g., turn on Pin 5) without affecting the others.

*   **Setting a Bit (OR `|`)**:
    ```c
    // Set bit 5
    REG |= (1 << 5);
    ```
*   **Clearing a Bit (AND `&` with NOT `~`)**:
    ```c
    // Clear bit 5
    REG &= ~(1 << 5);
    ```
*   **Toggling a Bit (XOR `^`)**:
    ```c
    // Toggle bit 5
    REG ^= (1 << 5);
    ```
*   **Checking a Bit (AND `&`)**:
    ```c
    if (REG & (1 << 5)) { /* Bit is set */ }
    ```
