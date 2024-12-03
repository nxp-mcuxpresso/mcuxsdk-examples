Hardware requirements
=====================
- Micro USB cable
- one IMX95LPD5-EVK board
- JLink Plus
- 12V~20V power supply
- Personal Computer

Board settings
==============
The example requires connecting between CAN pins of two boards.
The connection should be set as follows:

Between two boards:
- J18-2(CAN2_H), J17-2(CAN1_H)
- J18-3(CAN2_L), J17-3(CAN1_L)

Prepare the Demo
================
1.  Connect 12V~20V power supply and JLink Plus to the board, switch SW4 to power on the board
2.  Connect a micro USB cable between the host PC and the J31 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4. Use command “ make SOC=iMX95 flash_lpboot_sm_all OEI=YES “ to generate the flash.bin
   note:
	one patch should be applied manually, this will generate a new dtb file: imx95-19x19-evk-can1.dtb, use this dtb file to boot up linux.
5.  Download the program to the target board.
6.  Either re-power up your board or launch the debugger in your IDE to begin running the demo.
7.  After linux boot up, use the command to setup CAN: “ ip link set can0 up type can bitrate 500000 “,   here only support 500K bitrate, because M7 side use this bitrate. and run “ candump can0 & “  , to keep dump the received can data.
8. on M7 console(first console), type “a”, and enter    M7 side will send data 0 out, with can id = 0x321
9. on A core console(third console), you will see the CAN data received, the can id is 0x321, data is 0x00,
10. on A core console, use the command to send data to M7 side.  “ cansend can0 123#11 “
If every thing works well, you will see the data send out with can id 123, and also you will get a new received can data = 0x12 with can id = 0x321.
