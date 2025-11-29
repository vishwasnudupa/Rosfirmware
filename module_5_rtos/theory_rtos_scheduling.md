# Module 5: Real-Time Architecture (RTOS) - Theory Deep Dive

## 1. Why do we need an RTOS?

In the previous modules, we used a `while(1)` loop (Super-loop).
*   **Problem**: If the `CAN_Receive` function takes 100ms to process a message, the `Display_Update` function is delayed by 100ms. The system is **non-deterministic**.
*   **Solution**: An RTOS (Real-Time Operating System) allows us to break our code into independent **Tasks** and switch between them so fast that it *looks* like they are running in parallel.

## 2. The Scheduler

The Scheduler is the heart of the RTOS. It decides *which* task runs *when*.

### 2.1 Cooperative vs. Preemptive
*   **Cooperative**: Tasks voluntarily yield the CPU (`task_yield()`). If a task crashes or loops forever, the whole system freezes.
*   **Preemptive**: The Scheduler forcibly interrupts the running task (usually via a Timer Interrupt like **SysTick**) to switch to another task. This is safer and more common in automotive.

### 2.2 Round-Robin vs. Priority Based
*   **Round-Robin**: Task A -> Task B -> Task C -> Task A... (Fair share).
*   **Priority**: High Priority runs whenever it wants. Low Priority only runs when High Priority is sleeping.

## 3. Context Switching

How does the CPU switch from Task A to Task B?

1.  **Save Context**: The CPU pushes Task A's registers (R0-R15, PSR) onto Task A's **Stack**.
2.  **Switch Stack Pointer**: The OS changes the CPU's Stack Pointer (SP) to point to Task B's Stack.
3.  **Restore Context**: The CPU pops Task B's registers from Task B's Stack.
4.  **Resume**: The CPU executes the instruction where Task B left off.

## 4. RTOS Primitives

*   **Task**: A function with its own stack.
*   **TCB (Task Control Block)**: A struct holding the task's state (Stack Pointer, Priority, Status).
*   **Queue**: A thread-safe FIFO for sending data between tasks (e.g., CAN Task sends RPM to Display Task).
*   **Mutex**: A key to protect shared resources (e.g., preventing two tasks from writing to UART at the same time).
