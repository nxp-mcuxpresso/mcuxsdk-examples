This file contains the general information which can be shared by the examples on this board.
The example specific information can be found in *readme.md* or *example_board_readme.md*.

General Example Settings
========================
Hardware requirements
---------------------
- Micro USB cable
- MC56F83000-EVK board
- Personal Computer

Prepare the Demo
----------------
1.  Connect USB cable between the host PC and the JM60 USB(J8) port on the target board. It setups OSJTAG and COM port in PC device manager.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board with OSJTAG debug configuration.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
    Please be noted that default FCF(flash configuration field) setting makes MCU boot from bootloader. So after reset button press,
    it will wait 5s(wait in bootloader) to run the application code.
