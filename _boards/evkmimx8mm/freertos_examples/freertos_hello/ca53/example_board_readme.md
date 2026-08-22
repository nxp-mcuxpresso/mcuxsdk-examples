# Hardware requirements

- USB Type-C cable
- MIMX8MM-EVK board
- J-Link (optional)
- USB/UART converter (or the on-board debug UART)
- Personal computer

# Board settings

The Cortex-A53 debug console uses UART4, which is the M-Core UART header on the
MIMX8MM-EVK. Connect a USB/UART converter to UART4 (or use the default debug UART
routed to the on-board USB port).

# Prepare the demo

This example runs on the Cortex-A53 core and is loaded from DDR by U-Boot.

1. Build the `ddr_debug` or `ddr_release` configuration for `-Dcore_id=ca53`.
2. Copy the generated `freertos_hello_ca53.bin` to an SD card (or load it over TFTP).
3. Stop in U-Boot and load the binary to the DDR load address, then start it:

```
=> load mmc 1:1 0x93c00000 freertos_hello_ca53.bin
=> dcache flush
=> go 0x93c00000
```

# Running the demo

The log below shows the output of the example in the terminal window:

```
Hello world.
```
