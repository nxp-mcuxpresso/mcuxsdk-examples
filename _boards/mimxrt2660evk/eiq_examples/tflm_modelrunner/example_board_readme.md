Hardware requirements
=====================
- USB Type-C cable
- MIMXRT2660-EVK board
- Personal computer
- (Phase 2 / HTTP mode) Ethernet cable and a DHCP-capable network switch or router

Board settings
==============
- No special jumper changes are required for UART mode.
- For HTTP mode, connect an Ethernet cable to the RJ45 port (J5) labelled ETH1
  on the MIMXRT2660-EVK. The board will obtain an IP address via DHCP and
  print the assigned address on the serial console.

Prepare the Demo
================
1. Connect a USB Type-C cable between the host PC and the MCU-Link USB port
   (J19) on the target board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Build the xspi_nor_psram_debug (or xspi_nor_psram_release) target and
   download the program to the target board.
4. Either press the reset button (SW1) or launch the debugger in your IDE to
   begin running the demo.

UART mode (Phase 1)
===================
After reset the board prints a ModelRunner banner on the serial console.
Use the host-side scripts (scripts/cli.py or scripts/main.py) to load a
TFLite model and run inference:

  python scripts/cli.py --port <COMx/ttyUSBx> model_loadb <size>
  # ... paste binary model data ...
  python scripts/cli.py --port <COMx/ttyUSBx> run

The runner returns per-layer timing and the output tensor. Models containing
NeutronGraph custom ops are accelerated transparently via the Neutron NPU
(USE_NPU=1).

HTTP mode (Phase 2)
===================
Build with MODELRUNNER_HTTP=1 and USE_RTOS=1. The board starts a lightweight
HTTP server on port 10818. Use the ModelRunner HTTP API:

  PUT  http://<board_ip>:10818/v1         -- upload a .tflite model
  POST http://<board_ip>:10818/v1/run     -- run inference
  GET  http://<board_ip>:10818/v1/info    -- query model metadata

or drive the board with scripts/main.py:

  python scripts/main.py --address <board_ip> --model model.tflite

Note
====
IAR EWARM cannot debug the RT2660 directly. Use SEGGER Ozone to download and
debug the IAR-built image instead.
