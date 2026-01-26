# NBU Debug Module - How-To Guide

## What is the NBU Debug Module?

The NBU Debug module monitors the NBU (Narrow Band Unit) core from the host MCU in multi-core wireless systems. It provides:

- **Automatic fault detection** - Detects NBU crashes and asserts
- **Warning notifications** - Non-fatal issue tracking
- **Detailed crash analysis** - CPU state, fault type, execution context
- **HCI packet logging** - Optional BLE protocol capture for Wireshark

## Understanding the Dual Serial Output Architecture

The NBU Debug module uses **two separate serial outputs** for different purposes:

### 1. Main Console Output (JLink/Primary UART)
- **Format:** Human-readable ASCII text
- **Purpose:** Displays fault analysis, warnings, and diagnostic messages
- **Connection:** Typically connected via JLink debug port or primary UART
- **Always Active:** Available whenever the serial manager is enabled

**Example Output:**
```
=== NBU Fault/Assert Analysis ===
NBU Fault Detected
Processor State:
  PC  (Program Counter): 0x20001234
  ...
```

### 2. Debug Port Output (Secondary UART - HCI Logger)
- **Format:** Binary HCI raw packets
- **Purpose:** Captures BLE protocol packets for Wireshark analysis
- **Connection:** Dedicated UART port (requires physical connection to TX pin)
- **Optional:** Only active when HCI logging is explicitly enabled
- **Power Impact:** Not low-power friendly - for development debugging only

## Supported Platforms

- mcxw72evk
- frdmmcxw72
- kw47evk
- kw47loc

## Quick Start Integration

### Step 1: Add the Module to Your Project

**Enable the Board Debug NBU Module:**

Add the following Kconfig option to your project configuration (typically in `prj.conf` or `defconfig`):

```
CONFIG_MCUX_PRJSEG_module.board.wireless.board.debug_nbu=y
```

This will automatically add the following files to your project:

**Debug Files:**
- `board_debug_nbu.c` - Main implementation with fault/warning analysis
- `board_debug_nbu.h` - Public API and configuration


**Note:** These files are example implementations provided in the SDK. You can customize them for your specific needs or use them as-is for standard debugging scenarios.

### Step 2: Initialize at Startup

Include the board debug layer files in your application:

```c
#include "board_debug_nbu.h"
```

```c
int main(void) {
    // Initialize your hardware
    BOARD_InitHardware();
    
    // Initialize NBU debug framework
    BOARD_DbgNbuInit();
    
    // Continue with your application
    app_init();
    
    return 0;
}
```

### Step 3: Add Runtime Monitoring

**Recommended: Use FreeRTOS Idle Hook**

```c
void vApplicationIdleHook(void) {
    BOARD_DbgNbuProcess();  // Check NBU health
}
```

**Why Idle Hook?** It runs automatically when the system is idle without interfering with low power modes or forcing unnecessary wake-ups.

**Alternative Options:**

```c
// Option 2: Main Loop - BM use case
void main_loop(void) {
    while(1) {
        process_application_tasks();
        BOARD_DbgNbuProcess();
    }
}

// Option 3: Timer Callback
void system_timer_callback(void) {
    BOARD_DbgNbuProcess();
}
```

## Understanding the Output

### Normal Operation

When the NBU is healthy, you'll see no output. The module operates silently with minimal overhead.

### Warning Detection

**Basic Output (Level 1 - Default):**
```
WARNING: 2 New NBU Warnings detected
```

**Extended Output (Level 2):**

Configure in your project:
```c
#define BOARD_NBUDBG_NBU_WARNING_PRINT_LEVEL 2
```

Output includes warning IDs and BLE debug data:
```
WARNING: 2 New NBU Warnings detected
=== Warning Circular Table ===
2
->1    // Arrow shows most recent warning logged
0
...

BLE Debug Data (Raw - 256 bytes):
DBG_BLE_START
0000: 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
...
DBG_BLE_END
```

### Fault Detection - NBU Crash

```
=== NBU Fault/Assert Analysis ===

NBU Fault Detected
Exception Information:
  Exception ID: 0x00000003  
  NBU SHA1    : 0x12345678

Processor State:
  PC  (Program Counter): 0x20001234
  LR  (Link Register)  : 0x20001100
  SP  (Stack Pointer)  : 0x20002000
  PSR (Program Status) : 0x01000000

General Purpose Registers:
  R0:  0x00000001  R1:  0x20001500  R2:  0x00000000  R3:  0x00000010
  ...

Memory Management Faults Detected:
  - Data access violation
    Cause: Attempted to access a memory region without proper permissions

Execution Context:
  Mode: Thread Mode
  Thread Address: 0x20001234
  Thread Name: IDLE
```

### Assert Detection - Software Error

```
=== NBU Fault/Assert Analysis ===

NBU Assert Detected
  Line: 123
  File name: ble_controller.c
```

## HCI Packet Logging (Optional)

### When to Use

Enable HCI logging when you need to:
- Analyze BLE protocol issues
- Debug connection problems
- Capture air interface activity
- Review HCI command/event sequences

**Important:** HCI logging outputs **binary HCI data on a dedicated secondary UART port**, separate from the main console that displays human-readable analysis.

This feature:
- Continuously transmits binary data over the dedicated logging port
- Is **not low power friendly** - intended for **advanced debugging only**
- Designed for development and lab debugging scenarios

### Setup

**1. Enable in Your Project:**

To enable the feature, use the following configuration:
`CONFIG_MCUX_PRJSEG_module.board.wireless.board.debug_nbu.enable_hci_log=y`

**Important Configuration Notes:**

- **Separate UART Interface:** By default, HCI logs are transmitted on a **dedicated UART port**, separate from the main UART port
- **Debug Console Must Be Disabled:** Set `gDebugConsoleEnable_d=0` to avoid conflicts, as the HCI logger typically uses the same UART that would be used for debug console
- **Manual Connection Required:** You must physically connect to the HCI logging UART TX pin to capture the data

**Default UART Configuration:**

The UART used for HCI logging is defined by the `BOARD_DEBUG_UART_INSTANCE` macro in your board configuration files (typically in `board.h`).

For example on MCXW72 platforms, connect your USB-to-UART adapter to the following TX pins:

| Board          | TX Pin Location | UART Instance     |
|----------------|-----------------|-------------------|
| **mcxw72evk**  | J1 - Pin 3      | LPUART0 (default) |
| **frdmmcxw72** | J1 - Pin 2      | LPUART0 (default) |

**To verify or change the UART:**
Check the `BOARD_DEBUG_UART_INSTANCE` macro in your board's `board.h` file:
```c
#define BOARD_DEBUG_UART_INSTANCE 0U  /* LPUART0 */
```

**2. Capture Packets:**

Connect to your debug UART (typically the same port used for debug console output) and run:

```bash
python hci_to_btsnoop.py -p COM3 -b 115200 -o capture.btsnoop
```

**Note:** The tool captures HCI packets from the moment it starts running. For complete trace analysis, start the capture tool before running your application or before triggering the scenario you want to debug.

**3. Analyze in Wireshark:**

Open `capture.btsnoop` in Wireshark to view BLE protocol packets.

**4. Extract Debug Structures (Optional):**

```bash
# Auto-parse debug structures during capture
python hci_to_btsnoop.py -p COM3 -b 115200 -o capture.btsnoop --parse-debug
```

```bash
# Or parse existing capture file
python debug_struct_parser.py capture.btsnoop --format btsnoop
```

## Customizing the HCI Logging Port

The framework provides a default LPUART implementation for HCI packet logging, but you can customize it to use different hardware interfaces.

### Port Abstraction Layer

The HCI logger uses three functions defined in `board_debug_nbu_port.h`:

```c
// Initialize the debug port hardware
void BOARD_DbgNbuPortInit(void);

// Write data to the debug port
void BOARD_DbgNbuPortWrite(const uint8_t *data, uint32_t length);

// Reinitialize port - To be used after power down exit
void BOARD_DbgNbuPortReinit(void);
```

### Using a Different Port

**Create Custom Port Implementation**

By default the configuration `CONFIG_MCUX_PRJSEG_module.board.wireless.board.debug_nbu_hci_log_port_lpuart` is used.
To use a custom port, please select the following configuration:
`CONFIG_MCUX_PRJSEG_module.board.wireless.board.debug_nbu_hci_log_port_custom=y`.

Create your own port file (e.g., `board_debug_nbu_custom_port.c`), and reimplement
the APIs.

Then update your build system to add the custom port file to your project.

You can implement any interface that supports serial transmission.

### Important Considerations

⚠️ **Timing Requirements:**
- Slow writes can impact real-time BLE operations
- Consider using high baudrate or high-speed interfaces

⚠️ **Power Management:**
- Implement proper reinitialization in `BOARD_DbgNbuPortReinit()`
- Port must survive low power mode transitions

## Best Practices

### ✅ Do

- **Use Idle Hook** for monitoring (lowest power impact)
- **Check version compatibility** if you see unexpected output
- **Keep HCI logging disabled** in production unless needed

### ❌ Don't

- **Don't poll less/too frequently** - let idle hook handle it naturally
- **Don't enable HCI logging and debug console** on the same UART
- **Don't ignore warnings** - they indicate potential issues
- **Don't mix monitoring contexts** - `BOARD_DbgNbuProcess()` not thread-safe

## Troubleshooting

### Version Mismatch Warning

```
!! Host Debug version 0x0001 != NBU debug version 0x0000 !!
!! The following analysis may be incorrect !!
```

**Solution:** Update your NBU firmware or host debug module to matching versions.

### No Output When Fault Expected

**Check:**
1. Is `BOARD_DbgNbuInit()` called during startup?
2. Is `BOARD_DbgNbuProcess()` being called regularly?
3. Does your NBU firmware include fault handler support?

### HCI Logging Not Working - (Using default port)

**Check:**
1. Is `gDebugConsoleEnable_d` set to 0?
2. Is the correct UART port configured?
3. Is baud rate matching (115200 default)?
4. Are you connected to the debug UART port?

### Custom Port Issues

**Check:**
1. Is `BOARD_DbgNbuPortInit()` being called by `BOARD_DbgNbuInit()`?
2. Are clock sources/pins configured correctly for your peripheral?
3. Is `BOARD_DbgNbuPortReinit()` called after wake-up from low power (if re-init is needed)?

**NOTE** For complete API reference and advanced usage, see the detailed README files.
