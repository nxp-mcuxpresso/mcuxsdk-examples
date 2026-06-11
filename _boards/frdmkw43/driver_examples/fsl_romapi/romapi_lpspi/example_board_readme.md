Overview
========

The romapi_lpspi example shows how to use ROMAPI to operate program SPI flash:

The project is a simple demonstration program of the SDK FLASIAP driver. It erases and programs
a portion of on-board SPI flash memory. A message a printed on the UART terminal as various
operations on SPI flash memory are performed.

Hardware requirements
=====================
- Type-C USB cable
- FRDM-KW43 Board
- Personal Computer

Board settings
==============
- U30 and U12C affects the LPSPI1_SIN signal, causing it to remain at a low level.
  Note: Short JP6 to disable U30 before running this example.
- Ensure that ISP_SPI_CFG0 and ISP_SPI_CFG1 in IFR0 are configured to use pins PTB0, PTB1, PTB2, and PTB3.
  Note: You can update IFR0 to use PTB pins with the binary file:
  1. While holding pressed the **SW2** on the FRDM-KW43 board, attach the USB connector J28 to your computer. Then, release the **SW2** after you plugged the USB cable on your computer then silicon will run into ISP mode. If not using the FRDM-KW43 board, PTA5 must be pulled high before powering on the board to enter ISP mode.
  2. Verify which COM port is assigned to your FRDM-KW43 board. To check the assigned COM port, in the Windows **Device Manager** program, search for Ports **\(COM & LPT\)** and save the COM port number. In this example, the assigned COM port is **COM3**.
  3. blhost.exe -p COM3 -- flash-erase-region 0x01002000 0x1800
  4. blhost.exe -p COM3 -- write-memory 0x01002000 IFR0_ISP_SPI_CFG_SELECT_PTB0123.bin
  5. blhost.exe -p COM3 -- reset

Prepare the Demo
================
1. Connect a USB cable between the host PC and the FRDM board J28.
2. Open a serial terminal with the following settings:
   - 115200 baud rate
   - 8 data bits
   - No parity
   - One stop bit
   - No flow control
3. Download the program to the target board.
4. Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
These instructions are displayed/shown on the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~
ROM API LPSPI Example Start 

Initializing LPSPI flash 

LPSPI flash Information: 
LPSPI flash base address: 0, Hex: (0x0)
LPSPI flash page size: 256 B
LPSPI flash total size: 8192 KB, Hex: (0x800000)
Erase a sector of LPSPI flash
Successfully erased sector 0x7ff000 -> 0x800000

Program a buffer to a page of LPSPI flash 

Successfully programmed and verified location LPSPI flash 0x7ff000 -> 0x7ff200 

End of LPSPI flash Example 
~~~~~~~~~~~~~~~~~~~~~~~
And you will find the flash has been programed.
