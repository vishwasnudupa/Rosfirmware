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
    "-kernel", "../module_5_rtos/project/build/firmware.bin",
    "-serial", "tcp:127.0.0.1:1236,server,nowait" # Port 1236 for RTOS test
]

def test_rtos_multitasking():
    print("[TEST] Starting QEMU (Module 5: RTOS)...")
    process = subprocess.Popen(QEMU_CMD, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    
    try:
        time.sleep(1)
        print("[TEST] Connecting to UART...")
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("127.0.0.1", 1236))
        
        # We expect to see "System Booted" and then repeated "RPM:" messages
        boot_seen = False
        rpm_count = 0
        
        start_time = time.time()
        while time.time() - start_time < 5:
            data = s.recv(1024).decode('utf-8', errors='ignore')
            print(f"[RX] {data.strip()}")
            
            if "System Booted" in data:
                boot_seen = True
            if "RPM:" in data:
                rpm_count += 1
                
            if boot_seen and rpm_count >= 3:
                print("[PASS] RTOS Multitasking verified (Console Task running).")
                return

        print("[FAIL] Timed out. RTOS might be stuck.")

    except Exception as e:
        print(f"[ERROR] {e}")
    finally:
        print("[TEST] Stopping QEMU...")
        process.terminate()
        s.close()

if __name__ == "__main__":
    test_rtos_multitasking()
