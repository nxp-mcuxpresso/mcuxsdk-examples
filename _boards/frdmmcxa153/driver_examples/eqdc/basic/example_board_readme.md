Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA153 board
- Personal Computer
- An encoder with PHASE A/B signals.

Board settings
============
1. For pin connection between board and encoder,
      J3_3 -> IOMUXC_GPIO_AD_B0_09_GPIO1_IO09 -> kINPUTMUX_TrigIn10ToQdc0Phasea -> EQDC_PHA
      J3_1 -> IOMUXC_GPIO_AD_B0_10_GPIO1_IO10 -> kINPUTMUX_TrigIn5ToQdc0Phaseb -> EQDC_PHB
      J1_3 -> IOMUXC_GPIO_AD_B0_11_GPIO1_IO11 -> kINPUTMUX_TrigIn2ToQdc0Index -> EQDC_INDEX


Prepare the Demo
===============
1.  Connect a Type-C USB cable between the host PC and the MCU-Link port(J15) on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
${ANCHOR}
