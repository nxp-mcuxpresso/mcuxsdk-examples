# RSTP Example

## Overview
This example demonstrates how to use the NETC driver with RSTP (Rapid Spanning Tree Protocol) and MSTP (Multiple Spanning Tree Protocol) to create a managed Ethernet switch with spanning tree support.

## Features
- RSTP/MSTP protocol support using mstp-lib
- BPDU frame trapping and processing
- Dynamic port state management (Blocking/Learning/Forwarding)
- FDB (Forwarding Database) management
- Topology change detection and handling
- Zero-copy buffer management for efficient packet processing

## Hardware Requirements
- i.MX943 EVK board
- Multiple Ethernet cables for creating network topology
- Optional: Additional switches or bridges for testing spanning tree

## Software Requirements
- MCUXpresso SDK
- FreeRTOS
- NETC driver
- mstp-lib (included in project)

## Board Settings
1. Connect Ethernet cables to switch ports (Port 0-3)
2. Ensure PHY power is properly configured
3. Configure boot switches for the target core

## Running the Demo
1. Build and download the program to the target board
2. Connect multiple Ethernet devices to create a network topology
3. Observe RSTP convergence through debug console output
4. Monitor port states and topology changes

## Expected Behavior
- On startup, all ports begin in Blocking state
- RSTP protocol negotiates and ports transition to appropriate states
- Root bridge is elected based on bridge priority
- Ports become Forwarding or Blocking based on topology
- Topology changes are detected and handled automatically

## Debug Output
The example provides detailed logging of:
- BPDU reception and transmission
- Port state transitions
- Topology changes
- FDB updates
- Protocol state machine transitions

## Configuration
Key parameters can be modified in the source code:
- Bridge priority (default: 0x8000)
- Port priorities
- Path costs
- Hello time, Max Age, Forward Delay
- MSTP region configuration

## Notes
- This example requires FreeRTOS for task management
- Ensure sufficient heap and stack sizes are configured
- BPDU frames are trapped to management port for processing
- The example supports both RSTP and MSTP modes

## Troubleshooting
- If ports don't forward: Check cable connections and PHY status
- If BPDU not received: Verify BPDU trapping is enabled
- If convergence is slow: Check timer configurations
- For buffer allocation failures: Increase buffer pool size
- If port status changes periodically: Verify BPDU trapping is enabled on remote devices.

## References
- IEEE 802.1D-2004: RSTP specification
- IEEE 802.1Q-2018: MSTP specification
- i.MX943 Reference Manual: NETC chapter
- mstp-lib documentation: https://github.com/adigostin/mstp-lib

# Board-Specific Setup for RSTP Example

## i.MX943 EVK Board Configuration

### Hardware Setup
1. **Power Supply**
   - Connect 12V power adapter to the board
   - Ensure all power LEDs are lit

2. **Ethernet Connections**
   - Port 0: Connect to network device or another switch
   - Port 1: Connect to network device or another switch
   - Port 2: Connect to network device or another switch

3. **Debug Console**
   - Connect USB cable to debug UART port
   - Configure terminal: 115200 baud, 8N1, no flow control

4. **Boot Configuration**
   - Refer to board user guide for switch settings

### PHY Configuration
- The example uses the on-board Ethernet PHYs
- PHY addresses are auto-configured
- Link speed: Auto-negotiation (10/100/1000 Mbps)
- Duplex mode: Auto-negotiation (Half/Full)

### Memory Configuration
- Stack size: 8KB (0x2000)
- Heap size: 32KB (0x8000)
- Buffer pool: 16 buffers 2KB each
- Adjust if needed based on network traffic

### Testing Topology Examples

#### Simple Loop Test
```
    [PC1]
      |
   [Port0]
  [i.MX943]
   [Port1]
      |
    [PC2]
```

#### Redundant Path Test
```
        [Switch A]
         /      |
    [Port0]     |
 [i.MX943 EVK]  |
    [Port1]     |
         \      |
        [Switch B]
```

### Expected Console Output
```
RSTP Example Starting...
Initializing NETC switch...
Buffer ring initialized with 32 buffers
Wait PHY link up, please link up all switch ports.
...
RSTP: Bridge started
RSTP: Port 1 -> Designated Forwarding
RSTP: Port 2 -> Root Forwarding
...
```

## Supported Boards
- [IMX943-EVK](../../../_boards/imx943evk/driver_examples/netc/rstp_example/example_board_readme.md)
