# NBU Debug Framework

## Overview

The NBU Debug module is a comprehensive debugging service designed for monitoring, fault detection, and post-mortem analysis of the **NBU (Narrow Band Unit)** from the host MCU in multi-core wireless systems. This module provides real-time crash and warning detection, detailed fault analysis, and debug information extraction capabilities across different cores.

## Supported platforms
- mcxw72evk
- frdmmcxw72
- kw47evk
- kw47loc

## Architecture Overview

The NBU Debug Framework consists of two main layers:

### 1. Core NBU Debug Module (`middleware/wireless/framework/services/DBG/nbu_dbg/`)
The foundational layer providing cross-core communication and debug data structures.

### 2. Board Debug Layer (`examples/_common/project_segments/wireless/wireless_mcu/debug`)
The application layer providing ready-to-use initialization and processing functions with detailed fault/warning analysis.

Board Debug Layer is provided as **example code** to demonstrate how to use the core NBU Debug Framework and extract debug information from the NBU.
The current implementation outputs debug information to a UART serial interface, but this is just one possible implementation. The extraction procedures and fault analysis logic can be easily adapted to work with other interfaces.
This layer serves as a reference implementation to show the complete debug information extraction procedure.

```
Application Layer (Board Debug Layer)
├── board_debug_nbu.h/.c     # High-level API with fault and warning analysis
│
Core NBU Debug Module Layer
├── framework/services/DBG/nbu_dbg/
│   ├── common/              # Shared data structures and definitions
│   ├── host_interface/      # MCU/Host side implementation
│   └── nbu_interface/       # NBU side implementation

┌─────────────────────────────────────────────┐   ┌──────────────────────────────────────────────────┐
│                  HOST Side                  │   │                   NBU Side                       │
│ ┌─────────────────────────────────────────┐ │   │ ┌──────────────────────────────────────────────┐ │
│ │  Application Layer - Board Debug Layer  │ │   │ │                                              │ │
│ │                                         │ │   │ │                                              │ │
│ │  • High-level API with fault analysis   │ │   │ │        Fault handler / NBU Firmware          │ │
│ │  • Human-readable fault reporting       │ │   │ │                                              │ │
│ │  • Execution context analysis           │ │   │ │                                              │ │
│ └───────────────┬─────────────────────────┘ │   │ └───────────────┬──────────────────────────────┘ │
└─────────────────┼───────────────────────────┘   └─────────────────┼────────────────────────────────┘
                  │ uses                                            │ uses                            
                  ▼                                                 ▼                                 
┌─────────────────────────────────────────────┐   ┌──────────────────────────────────────────────────┐
│             NBU Debug Module Layer          │   │              NBU Debug Module Layer              │
│ ┌─────────────────────────────────────────┐ │   │ ┌──────────────────────────────────────────────┐ │
│ │  • Debug data access (host side)        │ │   │ │  • NBU Fault indication                      │ │
│ │  • NBU health check                     │ │   │ │  • CPU context capture (registers, stack...) │ │
│ │                                         │ │   │ │  • Protocol debug data collection            │ │
│ └───────────────┬─────────────────────────┘ │   │ └───────────────┬──────────────────────────────┘ │
└─────────────────┼───────────────────────────┘   └─────────────────┼────────────────────────────────┘
                  └─────────────────────────────────────────────────┘
                        Cross-core communication through shared memory
                        • Debug buffer in shared RAM
                        • Fault status flags and health indicators 
```

## How the Components Work Together

### 1. **Core Module Responsibilities**

- **Cross-Core Communication**: Establishes shared memory regions and communication between MCU and NBU
- **Fault/Warning Detection**: Ability to monitor NBU health status and detect both errors and warnings.
- **Memory Management**: Handles NBU power domain control and shared memory access coordination from the host.

### 2. **Board Debug Layer Responsibilities**

- **Intelligent Fault Analysis**: Comprehensive crash analysis with human-readable explanations
- **Detailed Reporting**: Serial console output with fault categorization
- **Execution Context Analysis**: Thread vs Handler mode detection
- **Protocol Debug Data**: Raw dump of BLE and 802.15.4 debug information

## Integration Workflow

### 1. **System Initialization**

```c
#include "board_debug_nbu.h"

int main(void) {
    // Initialize hardware and system
    hardware_init();
    
    // Initialize NBU debug framework
    BOARD_DbgNbuInit();  // Initialize NBU debug framework and register callback
    
    // Continue with application initialization
    app_init();
    
    return 0;
}
```

### 2. **Runtime Monitoring**

```c
// Option 1 (*recommended*): In Idle Hook, FreeRTOS example:
void vApplicationIdleHook(void) {
    BOARD_DbgNbuProcess();  // Check NBU health and process fault/warning detection
}

// Option 2: In Main Loop
void main_loop(void) {
    while(1) {
        process_application_tasks();
        BOARD_DbgNbuProcess();  // Periodic health check
        system_maintenance();
    }
}

// Option 3: In Timer Callback
void system_timer_callback(void) {
    BOARD_DbgNbuProcess();  // Regular health monitoring
}
```

**Why Option 1 is Recommended:** Option 1 (Idle Hook) is preferred due to low power constraints - it executes naturally when the system has no other tasks to run without interfering with low power state transitions, avoids forced wake-ups from the host MCU solely for NBU health checks (unlike timer-based approaches) and preserves power efficiency.

## Comprehensive Warning Analysis Features

### 1. **Warning Detection and Notification**
```
New NBU Warnings detected: 2 warnings
```

### 2. **Warning vs Error Handling**
The system differentiates between warnings and fatal errors:

- **Warnings (`nbu_warning_count > 0`)**:
  - Non-fatal issues detected on NBU
  - Simple count notification displayed

- **Fatal Errors (`nbu_error_count > 0`)**:
  - Critical faults requiring full analysis
  - Complete processor state dump
  - Detailed fault status analysis
  - Full debug information extraction

## Comprehensive Fault Analysis Features

```
NBU Debug version: 0x0001
!! Host Debug version 0x0001 != NBU debug version 0x0000 !!
!! The following analysis may be incorrect !!
```

The system automatically checks version compatibility between host and NBU debug structures to ensure accurate analysis.

### 1. **Exception and Assert Information Analysis**

#### **For NBU Faults:**
```
=== NBU Fault/Assert Analysis ===

NBU Fault Detected
Exception Information:
  Exception ID: 0x00000003  
  NBU SHA1    : 0x12345678
```

#### **For NBU Asserts:**
```
=== NBU Fault/Assert Analysis ===

NBU Assert Detected
  Line: 123
  File name: ble_controller.c
```

The system automatically distinguishes between hardware faults and software asserts based on the exception ID.

### 2. **Complete Processor State Dump**
```
Processor State:
  PC  (Program Counter): 0x20001234
  LR  (Link Register)  : 0x20001100
  SP  (Stack Pointer)  : 0x20002000
  PSR (Program Status) : 0x01000000

General Purpose Registers:
  R0:  0x00000001  R1:  0x20001500  R2:  0x00000000  R3:  0x00000010
  R4:  0x20002000  R5:  0x00000000  R6:  0x00000000  R7:  0x20001FF0
  ...
```

### 3. **Intelligent Fault Classification**

#### **Example for Memory Management Faults**
```
Memory Management Faults Detected:
  - Data access violation
    Cause: Attempted to access a memory region without proper permissions
```

### 4. **Execution Context Analysis**

#### **Thread Mode Faults**
```
Execution Context:
  Mode: Thread Mode
  Thread Address: 0x20001234
  Thread Name: IDLE
```

#### **Handler Mode Faults**
```
Execution Context:
  Mode: Handler Mode (Interrupt Context)
  IRQ Number: 10
```

### 5. **Protocol Stack Debug Data**

BLE Debug Data provides 256 bytes of BLE Link Layer specific debug information

```
BLE Debug Data (Raw - 256 bytes):
DBG_BLE_START
0000: 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
0010: 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
...
DBG_BLE_END
```

## Benefits

### **Production-Ready Design**
- **Non-Intrusive Operation**: The framework is designed to be used in production environments as it operates without interfering with normal system functionality
- **Zero Power Impact**: No impact on low power modes or power consumption during normal operation
- **Field Deployment Ready**: Provides valuable diagnostic capabilities for deployed products

#### **Rapid Debugging**
- **Rapid Fault/Assert/Warning Detection**: Immediate notification without polling
- **Complete Context**: Full CPU state and execution context at fault time
- **Assert Analysis**: File and line information for software asserts
- **Root Cause Analysis**: Detailed fault explanations with probable causes

#### **Protocol Stack Debugging**
- **BLE Stack Insights**: 256 bytes of BLE-specific debug data
- **Custom Protocol Support**: Extensible debug buffer system
- **Minimal Overhead**: Negligible impact on normal system performance and on low power consumption
- **Optimized Memory Usage**: Efficient memory layout with shared regions for fault and assert contexts

#### **Comprehensive Logging**
- **Human-Readable Output**: Less work for register decoding
- **Structured Information**: Consistent format for automated log processing
- **Complete Context**: All necessary information in one place

### **Performance Impact**
- **Normal Operation**: Very low overhead
- **Fault Detection**: Single API call per check
- **Debug Extraction**: Only executed after faults occur

### **Power Considerations**
- **Automatic Power Management**: Handles NBU domain wake-up for debug access
- **No Low Power Impact**: Fault detection doesn't require the NBU to be awaken.
- **Minimal Active Time**: Quick fault checks with immediate sleep return

## Platform Requirements

### **NBU Dependencies**
- NBU shall be built with framework fault handlers support
- Debug console for output (optional)

## Conclusion

The NBU Debug Framework provides a complete solution for NBU fault detection and analysis, combining:

- **Comprehensive analysis** with human-readable output for faults and asserts conditions
- **Easy integration** with minimal code changes and configurable output
- **Production-ready reliability** with minimal overhead
- **Flexible debugging** supporting both hardware faults and software asserts

This module significantly reduces debugging time, improves system reliability, and provides valuable insights for both development and field deployment.
