Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN947 board
- Personal Computer

Board settings
============

Prepare the Demo
===============
1.  Connect a type-c USB cable between the PC host and the MCU-Link USB port (J17) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================

When the example runs successfully, the following message is displayed in the terminal:

```
FLEXSPI NOR example started!

Successfully get FLEXSPI NOR configuration block

Successfully init FLEXSPI serial NOR flash

Serial NOR flash Information:
Serial NOR flash size: xx KB, Hex: (xx)
Serial NOR flash sector size: xx KB, Hex: (xx)
Serial NOR flash page size: xx B, Hex: (xx)

Erasing serial NOR flash over FLEXSPI

Successfully erased one sector of NOR flash device xx -> xx

Program a buffer to a page of NOR flash

Access serial NOR flash over AHB

Successfully programmed and verified location FLEXSPI AMBA memory xx -> xx

End of FLEXSPI NOR Example!
```

