# Verification & Simulation Guide

This course is designed to be verified using **QEMU** (Quick Emulator). We target the **MPS2-AN385** board (ARM Cortex-M3) because it is well-supported by QEMU.

## 1. Installing QEMU

### Windows
1.  Download the installer from [qemu.org](https://www.qemu.org/download/#windows).
2.  Add the installation path (e.g., `C:\Program Files\qemu`) to your System PATH.
3.  Verify by running:
    ```bash
    qemu-system-arm --version
    ```

### Linux (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install qemu-system-arm
```

## 2. Running Simulations Manually

You can run your firmware binary directly in the terminal.

### Module 2 (UART)
Navigate to the build directory and run:
```bash
qemu-system-arm -M mps2-an385 -cpu cortex-m3 -nographic -kernel firmware.bin
```
*   **-M mps2-an385**: The machine to simulate.
*   **-nographic**: Redirects the simulated UART to your terminal.
*   **Exit**: Press `Ctrl+A` then `x` to kill QEMU.

## 3. Running Automated Tests

We have provided Python scripts to automate the verification.

### Prerequisites
*   Python 3 installed.

### Running the UART Test
This script launches QEMU in the background, connects to the UART port via TCP, sends commands (`help`), and verifies the response.

1.  Build Module 2 first.
2.  Run the test:
    ```bash
    cd firmware_guide/tests
    python test_uart.py
    ```

### Running the CAN Test
Verifies that the CAN Controller is sending frames and receiving them back (Loopback).

1.  Build Module 4.
2.  Run the test:
    ```bash
    python test_can.py
    ```

### Running the RTOS Test
Verifies that the System boots and the Console Task is getting scheduled to print telemetry.

1.  Build Module 5.
2.  Run the test:
    ```bash
    python test_rtos.py
    ```

## 4. Troubleshooting
*   **"Guest has not initialized the display"**: We use `-nographic` so this is expected if you don't use that flag.
*   **"Connection refused"**: QEMU didn't start fast enough. Try increasing `time.sleep(1)` in the python script.
