Hardware requirements
=====================
- USB Type-C cable
- MIMXRT2660-EVK board (on-board APS512XXN-OBx9 16-bit Xccela PSRAM on XSPI1,
  used as the LLC-cacheable test region)
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1. Connect the USB cable between the PC host and the MCU-Link USB port on the board.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Launch the debugger in your IDE to begin running the demo.

Running the demo
================
When the example runs successfully, the LLC information, non-zero performance
counters and a final PASS are printed, for example:

~~~~~~~~~~~~~~~~~~~~~
LLC example start.
LLC driver version : 2.1.0
Cache line size    : 128 bytes
Ways               : 8
Sets               : 256
Total capacity     : 262144 bytes
...
LLC already initialized by boot code; leaving it untouched.
Range and whole-cache maintenance completed; data preserved.
Performance counters:
  cycles         : ...
  read requests  : ...
  ...

LLC example PASS.
~~~~~~~~~~~~~~~~~~~~~

Note
====
- The LLC test region is the on-board APS512XXN Xccela PSRAM behind XSPI1. The
  Boot ROM does not bring it up on the default targets, so the board port
  initialises it in software with xspi_hyper_ram_init() (reused from the
  xspi/psram polling_transfer driver example): global reset, mode-register
  setup, X16 entry and read-DLL re-arm at the device maximum of 250 MHz.
- The example uses the LLC-cached window at 0x88000000 for all CPU accesses.
  BOARD_InitHardware covers this window with an inner-non-cacheable /
  outer-write-back MPU region so the CM33 L1 D-cache stays out and every CPU
  burst reaches the LLC as an aligned line fill (unaligned bursts on the direct
  0x80000000 path corrupt at XSPI buffer-line boundaries on this silicon).
- On RT2660 the LLC is brought online by boot code (board.c
  BOARD_EarlyConfigLLC), so the example does not re-initialize it; it only
  ensures it is enabled before use.
