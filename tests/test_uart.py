import subprocess
import time
import socket
import sys

# Configuration
QEMU_CMD = [
    "qemu-system-arm",
    "-M", "mps2-an385",
    "-cpu", "cortex-m3",
    "-nographic",
    "-kernel", "../module_2_uart/project/build/firmware.bin",
    "-serial", "tcp:127.0.0.1:1234,server,nowait" # Expose UART as TCP server
]

def test_uart_cli():
    print("[TEST] Starting QEMU...")
    # Start QEMU in background
    process = subprocess.Popen(QEMU_CMD, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    
    try:
        # Give QEMU a moment to start
        time.sleep(1)
        
        print("[TEST] Connecting to UART...")
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("127.0.0.1", 1234))
        
        # Read initial banner
        data = s.recv(1024).decode('utf-8', errors='ignore')
        print(f"[RX] {data}")
        
        if "Virtual Instrument Cluster CLI" in data:
            print("[PASS] Banner detected.")
        else:
            print("[FAIL] Banner not found.")
            return

        # Send Command
        print("[TX] help\\n")
        s.send(b"help\n")
        time.sleep(0.5)
        
        # Read Response
        data = s.recv(1024).decode('utf-8', errors='ignore')
        print(f"[RX] {data}")
        
        if "Available commands" in data:
            print("[PASS] 'help' command working.")
        else:
            print("[FAIL] 'help' command failed.")

    except Exception as e:
        print(f"[ERROR] {e}")
    finally:
        print("[TEST] Stopping QEMU...")
        process.terminate()
        s.close()

if __name__ == "__main__":
    test_uart_cli()
