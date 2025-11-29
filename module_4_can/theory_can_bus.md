# Module 4: The Automotive Backbone (CAN) - Theory Deep Dive

## 1. CAN (Controller Area Network)

CAN is the nervous system of a car. It allows ECUs (Engine, Brakes, Dashboard) to talk to each other without a central host computer.

### 1.1 The Physics: Differential Signaling
CAN uses two wires: **CAN_H** (High) and **CAN_L** (Low).
*   **Recessive State (Logic 1)**: CAN_H = 2.5V, CAN_L = 2.5V. (Voltage difference = 0V).
*   **Dominant State (Logic 0)**: CAN_H = 3.5V, CAN_L = 1.5V. (Voltage difference = 2V).
*   **Why?** Noise immunity. If a spark plug fires and adds noise to both wires, the *difference* remains the same.

### 1.2 The Protocol: Arbitration
What happens if two ECUs try to talk at the same time?
*   **CSMA/CD+AMP**: Carrier Sense Multiple Access / Collision Detection + Arbitration on Message Priority.
*   **The Rule**: Logic 0 (Dominant) overwrites Logic 1 (Recessive).
*   **The ID**: Every message has an ID (e.g., 0x123). Lower ID = Higher Priority.
*   **The Fight**:
    *   ECU A sends ID 0x100 (Binary ...000).
    *   ECU B sends ID 0x110 (Binary ...001).
    *   At the last bit, ECU A sends '0' and ECU B sends '1'.
    *   The bus sees '0'.
    *   ECU B reads back '0', realizes it lost, and shuts up. ECU A continues.

### 1.3 The Frame Format
*   **ID**: 11-bit (Standard) or 29-bit (Extended).
*   **DLC (Data Length Code)**: How many bytes (0-8).
*   **Data**: The payload.
*   **CRC**: Checksum.
*   **ACK**: The sender sends a Recessive bit. *Any* ECU that received the message correctly pulls the line Dominant to say "Got it!".

### 1.4 Filters & Masks
An ECU doesn't care about *every* message. The Brake Controller doesn't care about the Radio Station.
*   **Hardware Filtering**: The CAN controller can be programmed to ignore messages that don't match a specific ID pattern, saving the CPU from waking up.

## 2. Bit Timing
CAN is synchronous but without a clock line. Nodes synchronize on the edges of the signal.
*   **Quanta**: The smallest time unit.
*   **Segments**: Sync_Seg, Prop_Seg, Phase_Seg1, Phase_Seg2.
*   **Sample Point**: Usually at 87.5% of the bit time.
