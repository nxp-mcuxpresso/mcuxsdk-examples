Hardware requirements
===================
- FRDM-MCXA577 board
- USB cable
- 10BASE-T1S network or loopback-capable lab setup
- Serial terminal on the debug UART

Board settings
==============
This example uses the internal 10BASE-T1S digital PHY (TENBASET_PHY0) connected to ENET0.
PLCA is enabled with node ID 1.

Prepare the Demo
================
1. Connect a USB cable between the host PC and the board debug USB connector.
2. Open a serial terminal at 115200-8-N-1.
3. Connect the board to a compatible 10BASE-T1S network.
4. Build and flash the example.
5. Reset the board.

Running the demo
================
When the demo runs, it initializes the CMSIS ENET MAC and PHY drivers, waits for link up,
and then transmits broadcast frames while reporting successful transmit and receive events.
