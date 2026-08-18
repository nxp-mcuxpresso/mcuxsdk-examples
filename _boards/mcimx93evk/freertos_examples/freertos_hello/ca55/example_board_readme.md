# FreeRTOS hello (CA55)

## Hardware requirements

- USB Type-C cable
- MCIMX93-EVK board
- J-Link (optional)
- Personal computer

## Board settings

No special settings are required.

## Running the demo on Cortex-A55

The CA55 core on i.MX93 executes from DDR only. The image must be loaded and
started from U-Boot.

1. Connect the debug UART (LPUART2) to the PC and open a serial terminal
   (115200 baud, 8N1, no flow control).
2. Copy `freertos_hello_ca55.bin` to an SD card or load it over the network.
3. From the U-Boot prompt, load the binary to the DDR entry address and jump:

   ```
   => load mmc 1:1 0xd0000000 freertos_hello_ca55.bin
   => dcache flush
   => go 0xd0000000
   ```

4. The following message is shown on the debug console:

   ```
   Hello world.
   ```
