Hardware requirements
=====================
- Type-C USB cable
- FRDM-IMXRT700 board
- Personal Computer
- RaspberryPi 7 inch panel (800x480)

Board settings
============
To use the Raspberry Pi panel:
Connect the panel to J8. Then connect the panel's 5V pin to JP43-1, GND pin to JP43-2.

Prepare the Demo
===============
1.  Connect a Type-C USB cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build the project, the project uses the Raspberry Pi 7 inch panel.
4.  Download the program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
When the example runs, you can see a tiger with high render quality on white buffer.

Running the ARMGCC demo under GDB
=================================

Assuming your $PATH variable points to arm-none-eabi-gdb:

    $ arm-none-eabi-gdb release/openvg_tiger_freertos_cm33_core0.elf
    target remote localhost:2331
    monitor reset
    load
    monitor go
    continue
