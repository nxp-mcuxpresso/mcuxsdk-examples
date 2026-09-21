Hardware requirements
=====================
- USB Type-C cable
- MIMXRT2660-EVK board (on-board 16-bit DDR PSRAM on XSPI1, used as the
  LLC-cacheable test region)
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
- The LLC test region is the on-board DDR PSRAM behind XSPI1. This example
  defines USE_PSRAM (see reconfig.cmake) so the boot header carries the board's
  XMCD and the Boot ROM brings the PSRAM up before the image runs. Which XMCD is
  linked depends on USE_PSRAM_W958D6 in the board CMakeLists: 1 selects the
  Winbond W958D6NMYA4I (HyperBus, 250 MHz SCK), 0 selects the AP Memory Xccela
  part (APS256XXN/APS512XXN depending on board variant, 200 MHz SCK). It must
  match the part actually populated on your board - the MIMXRT2660-EVK ships
  either one depending on board variant.
- If the ROM leaves XSPI1 without a live DDR PSRAM (XSPI_MCR[X16_EN] clear),
  the LLC test region is not usable and LLC_ExamplePlatformInit() says so,
  naming USE_PSRAM_W958D6. On a working board that can only mean the linked
  XMCD does not match the populated part, so the fix is to correct that macro
  and rebuild; the example carries no software bring-up sequence, and none
  would work on both variants.
- The example uses the LLC-cached window at 0x88000000 for all CPU accesses.
  BOARD_InitHardware covers this window with an inner-non-cacheable /
  outer-write-back MPU region so the CM33 L1 D-cache stays out and every CPU
  burst reaches the LLC as an aligned line fill (unaligned bursts on the direct
  0x80000000 path corrupt at XSPI buffer-line boundaries on this silicon).
- On RT2660 the LLC is brought online by boot code (board.c
  BOARD_EarlyConfigLLC), so the example does not re-initialize it; it only
  ensures it is enabled before use.
