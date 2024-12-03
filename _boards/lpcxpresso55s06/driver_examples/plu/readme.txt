## Overview
-----------------------------------------------------------------------------------------
The PLU project is a simple demonstration program of the SDK PLU driver. In this example, a number of 
switches are used act as PLU inputs and LEDs are used to monitor the PLU outputs to demonstrate the 
configuration and use of the Programmable Logic Unit (PLU).

## Functional Description
-----------------------------------------------------------------------------------------
The example will configure the SWM for PLU inputs, outputs, and clock input, then configure the PLU module 
to make it work. 

(1) Firstly, the example selects plu_input(3) to be connected to LUT0 input 0 and set Truth table value 
0x00000001, then selects LUT_out(0) to be connected to PLU output 0. When plu_input(3) inputs low level, 
PLU_OUTPUT0 outputs high level and LED D7 turns on. When plu_input(3) inputs high level, PLU_OUTPUT0 outputs 
low level and LED D7 turns off.

(2) Secondly, the example selects LUT_out(0) to be connected to LUT1 input 0 and plu_input(4) to be connected 
to LUT1 input 1, set Truth table value 0x00000008, then selects LUT_out(1) to be connected to PLU output 1. 
When LUT_out(0) outputs high level (plu_input(3) inputs low level) and plu_input(4) inputs high level, PLU_OUTPUT1
outputs high level and LED D1 turns on. Otherwise, PLU_OUTPUT1 outputs low level and LED D1 turns off.

(3) Thirdly, the example selects plu_input(5) to be connected to LUT2 input 0 and set Truth table value 0x00000001, 
then selects Flip-Flops state(2) to be connected to PLU output 2. When plu_input(5) inputs low level, PLU_OUTPUT2 
outputs high level and LED D2 turns on. When plu_input(5) inputs high level, PLU_OUTPUT2 outputs low level and LED D2 
turns off.

## Toolchain Supported
---------------------
- IAR embedded Workbench 8.22.1
- Keil MDK 5.24a
- MCUXpresso10.2.0

## Hardware Requirements
------------------------
- Mini/micro USB cable
- LPCXpresso55S06 board
- Personal Computer

## Board Settings
------------------------
PLU shield board is required.
To use a Flip-Flop, the PIO0_1 (connect PIO0_1 to any clock by JP12) must be driven externally by the oscillator
on the PLU shield board.
Don't connect two pins of JP1 on the PLU shield board.

The jumper setting:
    Default jumpers configuration does not work,  you will need to add JP20 and JP21 (JP22 optional for ADC use)

## Run the Project
------------------------
1. Connect a micro USB cable between the PC host and the CMSIS DAP port(CN2 on the board).

2. Choose an IDE, building the project and download the program to the target board.
   More information about how to compile and program the project can refer to the 

   [Getting Started with MCUXpresso SDK](../../../../docs/Getting Started with MCUXpresso SDK.pdf).

3. Launch the debugger in your IDE to begin running the project.

## Expected Result
------------------------
When the example runs successfully, You will see the following phenomenon on the PLU shield board.
1. Set S3 'GND', LED D7 turns on. Set S3 'VCC', LED D7 turns off.
2. Set S3 'GND' and set S4 'VCC', LED D1 turns on. Otherwise, LED D1 turns off.
3. Set S5 'GND', LED D2 turns on. Set S5 'VCC', LED D2 turns off.

