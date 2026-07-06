# NBU Debug Framework

## Overview

The NBU Debug module is a comprehensive debugging service designed for monitoring, fault detection, and post-mortem analysis of the **NBU (Narrow Band Unit)** from the host MCU in multi-core wireless systems. This module provides real-time crash and warning detection, detailed fault analysis, debug information extraction capabilities, and HCI packet logging.

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
├── board_debug.h/.c               # Board init helpers (SWO, serial consoles)
├── board_debug_utils.h/.c         # Shared helpers (Base64 encode, hex dump, console write)
├── board_debug_nbu.h/.c           # High-level NBU API with fault and warning analysis
├── board_debug_nbu_port.h         # HCI logger port interface
├── board_debug_nbu_lpuart_port.c  # LPUART implementation for HCI logging (dedicated second UART)
├── board_debug_nbu_serialmgr_port.c # Serial Manager implementation for HCI logging (main serial port)
├── board_debug_coredump.h/.c      # App-core (Cortex-M33) Zephyr coredump "other" backend: streams the dump as '#CD:' framed Base64 over the application console
├── hci_to_btsnoop.py              # Python tool to capture HCI logs to BTSNOOP format
├── debug_struct_parser.py         # Python tool to parse debug structures from BTSNOOP files
├── board_debug_coredump_decode.py # Python tool to decode the '#CD:' coredump block to a binary Zephyr coredump
└── board_debug_parse.py           # Python tool: unified console parser (auto-routes NBU HCI, coredump, plain text)
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
│ │  • HCI packet logging                   │ │   │ │                                              │ │
│ └───────────────┬─────────────────────────┘ │   │ └───────────────┬──────────────────────────────┘ │
└─────────────────┼───────────────────────────┘   └─────────────────┼────────────────────────────────┘
                  │ uses                                            │ uses
                  ▼                                                 ▼
┌─────────────────────────────────────────────┐   ┌──────────────────────────────────────────────────┐
│             NBU Debug Module Layer          │   │              NBU Debug Module Layer              │
│ ┌─────────────────────────────────────────┐ │   │ ┌──────────────────────────────────────────────┐ │
│ │  • Debug data access (host side)        │ │   │ │  • NBU Fault indication                      │ │
│ │  • NBU health check                     │ │   │ │  • CPU context capture (registers, stack...) │ │
│ │  • HCI packet capture callback          │ │   │ │  • Protocol debug data collection            │ │
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
- **HCI Packet Capture**: Provides callback mechanism for capturing HCI packets from the NBU

### 2. **Board Debug Layer Responsibilities**

- **Intelligent Fault Analysis**: Comprehensive crash analysis with human-readable explanations
- **Detailed Reporting**: Serial console output with fault categorization
- **Execution Context Analysis**: Thread vs Handler mode detection
- **Protocol Debug Data**: Raw dump of BLE and 802.15.4 debug information
- **Warning ID Tracking**: Display of warning IDs from circular buffer
- **HCI Packet Logging**: Optional HCI packet capture for trace analysis

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

## Version Compatibility Check

The system automatically checks version compatibility between host and NBU debug structures to ensure accurate analysis.

Example of the output in case the versions are different:

```
NBU Debug version: 0x0001
!! Host Debug version 0x0001 != NBU debug version 0x0000 !!
!! The following analysis may be incorrect !!
```

## Comprehensive Warning Analysis Features

### 1. **Warning Detection and Notification**

The verbosity of warning output can be controlled using:

```c
// Level 1 (default): Print warning count only
#define BOARD_NBUDBG_NBU_WARNING_PRINT_LEVEL 1

// Level 2: Print warning count + warning IDs + BLE debug data
#define BOARD_NBUDBG_NBU_WARNING_PRINT_LEVEL 2
```

**Level 1 (Default):**
```
WARNING: 2 New NBU Warnings detected
```

**Level 2 (Extended):**
```
WARNING: 2 New NBU Warnings detected
=== Warning Circular Table ===
2
->1
0
0
...

BLE Debug Data (Raw - 256 bytes):
DBG_BLE_START
0000: 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10
0010: 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20
...
DBG_BLE_END
```

The arrow (`->`) indicates the current position in the circular warning buffer.

### 2. **Warning vs Error Handling**
The system differentiates between warnings and fatal errors:

- **Warnings (`nbu_warning_count > 0`)**:
  - Non-fatal issues detected on NBU
  - Warning count notification
  - Optional warning ID display (Level 2)
  - Optional BLE debug data dump (Level 2)

- **Fatal Errors (`nbu_error_count > 0`)**:
  - Critical faults requiring full analysis
  - Complete processor state dump
  - Detailed fault status analysis
  - Full debug information extraction

## Comprehensive Fault Analysis Features

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

The framework provides detailed analysis for three fault categories:

- **Memory Management Faults**: Access violations, MPU faults
- **Bus Faults**: Instruction/data bus errors, precise/imprecise faults
- **Usage Faults**: Undefined instructions, division by zero, unaligned access

Each fault includes probable cause explanation.

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

## HCI Packet Logging Feature

### Overview

The HCI logger captures raw BLE HCI packets from the NBU and outputs them to a dedicated UART port. This enables real-time protocol analysis using Wireshark or other Bluetooth analysis tools.

**Debug Events Over HCI:** In addition to standard HCI packets, the framework also sends debug-specific events as HCI vendor events:
- **Debug Structure Events**: Complete debug information (fault/assert context, registers, BLE debug data)

**Power Consideration:** HCI logging is **not power-friendly** as it continuously captures and transmits all HCI packets over the logging port in real-time. HCI logger should be used for development and debugging purposes only, and disabled in production or low-power deployments.

### Architecture

```
┌──────────────┐    ┌──────────────┐    ┌─────────────────────┐    ┌────────────────────────────────────┐    ┌───────────┐
│ HCI RX/TX    │───>│ Board Debug  │───>│   Debug UART Port   │───>│ Python Tool: hci_to_btsnoop.py     │───>│ Wireshark │
│ (Debug core) |    │    Layer     |    │ (Binary HCI Stream) │    │ (Serial Capture → BTSNOOP File)    │    │ Analysis  │
└──────────────┘    └──────────────┘    └─────────────────────┘    └────────────────────────────────────┘    └───────────┘
```
### Setup and Usage

#### 1. **Enable HCI Logger**

In your project configuration:

- Use the following configuration:
`CONFIG_MCUX_PRJSEG_module.board.wireless.board.debug_nbu.enable_hci_log=y`

- Make sure that Debug console is disabled to avoid conflict on the same UART:

```c
#define gDebugConsoleEnable_d 0  // Required to avoid UART conflicts
```

This reserves the debug UART port exclusively for binary HCI packet logging.

#### 2. **Capture HCI Packets**

Use the provided Python tool to capture packets:

```bash
# Capture from serial port and generate a btsnoop file called capture.btsnoop
python hci_to_btsnoop.py -p COM3 -b 115200 -o capture.btsnoop

# Same with auto-parsing the debug structures after capture
python hci_to_btsnoop.py -p COM3 -b 115200 -o capture.btsnoop --parse-debug
```

#### 3. **Analyze in Wireshark**

Open the generated `.btsnoop` file in Wireshark for protocol analysis.

#### 4. **Parse Debug Structures**

Extract and analyze NBU debug structures from captured data:

```bash
# Parse from BTSNOOP file
python debug_struct_parser.py capture.btsnoop --format btsnoop
```

Or use `hci_to_btsnoop.py` with the `-- auto-parse` feature direcly.

```bash
# Capture the HCI logs and auto-parse the debug structures after capture
python hci_to_btsnoop.py -p COM3 -b 115200 -o capture.btsnoop --parse-debug
```

### HCI Logger Port Implementation

The framework provides a port abstraction layer (`board_debug_nbu_port.h`) with LPUART implementation:

- **BOARD_DbgNbuPortInit()**: Initialize debug UART port for HCI logging
- **BOARD_DbgNbuPortWrite()**: Write HCI packet data
- **BOARD_DbgNbuPortReinit()**: Reinitialize after power down exit

Custom port implementations can be created by implementing these three functions.

### Packet Format

Each captured packet follows this format:
```
[direction(1 byte)][packet_type(1 byte)][packet_data...]
```

- **direction**: 0x00=TX (host→controller), 0x01=RX (controller→host)
- **packet_type**: 0x01=CMD, 0x02=ACL, 0x04=EVENT, 0x05=ISO

### Base64 Framing (shared serial port)

When the HCI log is routed to the **main serial port** (the `serialmgr` port), it shares
the line with human-readable console text. Raw binary HCI would corrupt that text stream,
so the firmware can Base64-encode each packet and emit it as a single ASCII-safe line:

```
@<base64( [direction][packet_type][packet_data...] )>\n
```

- The leading `@` is a single-character marker that is not part of the Base64 alphabet,
  so the host tool can unambiguously pick packet lines out of the mixed text stream.
- The direction byte, packet type and payload are concatenated and encoded as one unit,
  giving exactly one line (and one marker) per packet.

Enable it with the Kconfig option
`CONFIG_MCUX_PRJSEG_module.board.wireless.board.debug_nbu.enable_hci_log_base64=y`
(defaults on for the `serialmgr` port; it sets `-DBOARD_NBUDBG_HCI_LOG_BASE64=1`).

On the host side, run the capture tool in Base64 mode so it decodes these lines and
ignores the surrounding console text:

```bash
python hci_to_btsnoop.py -p COM3 -b 115200 -o capture.btsnoop --base64
```

The dedicated second UART (`lpuart` port) carries binary HCI only, so Base64 is not needed
there and the tool is used without `--base64`.

#### Processing a saved log file (offline)

Because Base64 lines are plain ASCII, a console log captured from the main serial port
can be saved to a text file and converted to BTSNOOP later, without being connected to
the board. Pass the log file with `-i/--input` instead of `-p/--port`:

```bash
# Generate a btsnoop from a previously saved console log
python hci_to_btsnoop.py -i console_log.txt --base64 -o capture.btsnoop

# Same, and auto-parse the debug structures afterwards
python hci_to_btsnoop.py -i console_log.txt --base64 -o capture.btsnoop --parse-debug
```

Notes:
- `--input` requires `--base64`. Offline file processing only supports the Base64 line
  framing.
- `--input` and `--port` are mutually exclusive: choose either live capture or file
  processing.

## Unified Console Parser (`board_debug_parse.py`)

When everything is routed to the **main application console** (the `serialmgr`
port with Base64 framing), a single capture can interleave three completely
different data streams plus ordinary application logs:

1. **NBU HCI logging** - lines prefixed with `@`
   (`@<base64(direction||packet_type||payload)>`). The radio core (NBU) reports
   its faults/asserts as HCI vendor events; these are turned into a BTSNOOP file
   and then analysed by `debug_struct_parser.py`. The NBU never emits a Zephyr
   coredump.
2. **App-core (Cortex-M33) Zephyr coredump** - a block framed with `#CD:`
   (`#CD:BEGIN#` / `#CD:<base64>` ... / `#CD:END#`). This is decoded to a binary
   Zephyr coredump consumable by `coredump_parser/log_parser.py` and
   `coredump_gdbserver.py`.
3. **Plain application text** - banners, `PRINTF` output, shell prompts, etc.

`board_debug_parse.py` is a single host entry point that ingests one full
console capture (file) or a live serial port, auto-detects which streams are
present, and dispatches every line to the correct decoder. It does not
re-implement any logic: it reuses the existing sibling decoders
(`debug_struct_parser.py`, `board_debug_coredump_decode.py`) and the vendored
`coredump_parser/log_parser.py`, and embeds a small self-contained BTSNOOP
writer so the offline path needs no pyserial. It is robust to arbitrarily
interleaved application logs and to decoy lines such as `@ not-a-packet` or
`garbage #CD:`.

### Usage

```bash
# Offline: parse a previously captured full application log.
python board_debug_parse.py -i capture.log

# Offline with named outputs + NBU BLE extension + ELF symbol resolution.
python board_debug_parse.py -i capture.log \
    --btsnoop nbu.btsnoop --coredump coredump.bin \
    --extension ble --elf build_vero/frdmkw43/lp_refdes_freertos/<app>.elf

# Live: open the COM port, capture until Ctrl+C, then parse everything.
python board_debug_parse.py -p /dev/ttyACM0 -b 115200 --save-log capture.log

# After a coredump is produced, launch the offline GDB server (needs ELF).
python board_debug_parse.py -i capture.log --coredump coredump.bin \
    --elf <app>.elf --gdb
```

`-i/--input` and `-p/--port` are mutually exclusive and one is required. The
script first prints a capture summary (how many NBU HCI, coredump, and plain
lines it saw), then runs only the decoders for the streams actually present.
Default outputs are `<base>.btsnoop` and `<base>.coredump.bin`; override with
`--btsnoop` / `--coredump`.

### When to use which tool

- Use **`board_debug_parse.py`** for the everyday case: a full main-console
  capture that may contain NBU HCI logging and/or an app-core coredump mixed
  with application text. It is the recommended single entry point.
- The individual scripts (`hci_to_btsnoop.py`, `debug_struct_parser.py`,
  `board_debug_coredump_decode.py`) remain fully usable standalone - for
  example when capturing the dedicated second `lpuart` HCI port (binary, no
  Base64), or when you only need one specific output.

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
- **Warning Tracking**: Circular buffer of warning IDs for trend analysis

#### **Protocol Stack Debugging**
- **BLE Stack Insights**: 256 bytes of BLE-specific debug data
- **HCI Packet Capture**: Real-time HCI packet logging
- **Custom Protocol Support**: Extensible debug buffer system
- **Minimal Overhead**: Negligible impact on normal system performance and on low power consumption
- **Optimized Memory Usage**: Efficient memory layout with shared regions for fault and assert contexts

#### **Comprehensive Logging**
- **Human-Readable Output**: Less work for register decoding
- **Structured Information**: Consistent format for automated log processing
- **Complete Context**: All necessary information in one place
- **Wireshark Integration**: Standard BTSNOOP format for industry-standard tools

### **Performance Impact**
- **Normal Operation**: Very low overhead
- **Fault Detection**: Single API call per check
- **Debug Extraction**: Only executed after faults occur
- **Asynchronous Processing**: Uses work queue to avoid blocking critical paths

### **Power Considerations**
- **Automatic Power Management**: Handles NBU domain wake-up for debug access
- **No Low Power Impact**: Fault detection doesn't require the NBU to be awaken.
- **Minimal Active Time**: Quick fault checks with immediate sleep return
- **UART Reinitialization**: Automatic UART reconfiguration after power down exit

## Platform Requirements

### **NBU Dependencies**
- NBU shall be built with framework fault handlers support

## Conclusion

The NBU Debug Framework provides a complete solution for NBU fault detection and analysis, combining:

- **Comprehensive analysis** with human-readable output for faults and asserts conditions
- **Warning tracking** with configurable verbosity levels
- **HCI packet logging** for protocol-level debugging with Wireshark integration
- **Easy integration** with minimal code changes and configurable output
- **Production-ready reliability** with minimal overhead
- **Flexible debugging** supporting both hardware faults and software asserts
- **Python tooling** for offline analysis and packet capture

This module significantly reduces debugging time, improves system reliability, and provides valuable insights for both development and field deployment.
