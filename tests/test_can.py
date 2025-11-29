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
    "-kernel", "../module_4_can/project/build/firmware.bin",
    "-serial", "tcp:127.0.0.1:1235,server,nowait" # Port 1235 for CAN test
]

def test_can_loopback():
    print("[TEST] Starting QEMU (Module 4: CAN)...")
    process = subprocess.Popen(QEMU_CMD, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    
    try:
        time.sleep(1)
        print("[TEST] Connecting to UART...")
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect(("127.0.0.1", 1235))
        
        # We expect to see "TX: RPM Frame Sent" and "RX: Loopback Received"
        tx_seen = False
        rx_seen = False
        
        start_time = time.time()
        while time.time() - start_time < 5: # Listen for 5 seconds
            data = s.recv(1024).decode('utf-8', errors='ignore')
            print(f"[RX] {data.strip()}")
            
            if "TX: RPM Frame Sent" in data:
                tx_seen = True
            if "RX: Loopback Received" in data:
                rx_seen = True
                
            if tx_seen and rx_seen:
                print("[PASS] CAN Loopback verified.")
                return

        print("[FAIL] Timed out waiting for CAN messages.")

    except Exception as e:
        print(f"[ERROR] {e}")
    finally:
        print("[TEST] Stopping QEMU...")
        process.terminate()
        s.close()

if __name__ == "__main__":
    test_can_loopback()
