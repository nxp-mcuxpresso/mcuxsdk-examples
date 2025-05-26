# Generic NBU Application Architecture for ThreadX RTOS

## Overview

This architecture provides a modular and reusable base for embedded applications running on the NBU using the ThreadX RTOS. It defines a clear separation between system-level initialization and application-specific logic, enabling developers to build consistent and maintainable applications.

## Architecture Components

### 1. **System Base (`main.c`)**

The base implementation handles:

- **Hardware Initialization**: Performed before the RTOS starts.
- **RTOS Startup**: Initializes ThreadX and defines application threads.
- **Thread Management**: Creates two threads:
  - **Idle Task**: Runs continuously when no other threads are active.
  - **Main Task**: Executes the core application logic.

### 2. **Application Integration Points**

To integrate with this architecture, an application must implement the following functions:

| Function               | Purpose                                                                 |
|------------------------|-------------------------------------------------------------------------|
| `APP_PreKernelInit()`  | Initializes components before the RTOS starts (e.g., memory, HAL).      |
| `APP_PostKernelInit()` | Initializes services after the RTOS is running (e.g., IPC, frameworks). |
| `APP_Main()`           | Contains the main application logic.                                    |
| `APP_Idle()`           | Defines behavior when the system is idle.                               |

## Initialization Flow

```
+---------------------+
|     main()          |
|---------------------|
| Init Hardware       |
| APP_PreKernelInit() |
| Start ThreadX       |
+----------+----------+
           |
           v
+---------------------------+
| tx_application_define()   |
|---------------------------|
| Init SysTick              |
| Create Idle Thread        |
| Create Main Thread        |
+----------+----------------+
           |
           +---------------------------+
           v                           v
+---------------------+     +---------------------+
|   idle_task()       |     |   main_task()       |
|---------------------|     |---------------------|
| APP_Idle() loop     |     | APP_PostKernelInit()|
|                     |     | APP_Main()          |
+---------------------+     +---------------------+
```

## Usage Example (Template)

Here’s a generic template showing how an application might implement the required hooks:

```c

#include "app.h"

void APP_PreKernelInit(void)
{
    // Initializes components before the RTOS starts (e.g., memory, HAL).
    // RTOS primitives are not available
}

void APP_PostKernelInit(void)
{
    // Initializes services after the RTOS is running (e.g., IPC, frameworks)
    // RTOS primitives are available
}

void APP_Main(void)
{
    // Main application logic
    // This could be a loop or a one-time setup
    // Example: while (1) { process_events(); }
    // Returning from this function will terminate the thread
}

void APP_Idle(void)
{
    // Optional: Enter low-power mode
    // Example: __WFI(); // Wait For Interrupt
}
```

This template is intended to be copied and customized by developers to suit their specific application needs.
