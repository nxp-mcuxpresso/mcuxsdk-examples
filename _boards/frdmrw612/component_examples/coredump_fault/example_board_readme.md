Hardware requirements
=====================
- Micro USB cable
- FRDM-RW612 board
- Personal Computer

Board settings
============

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-Link USB port (J10) on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
Note: Once the program is download to the target board, please press SW1(Reset Button) to reset program.

Expected log:  
Coredump Fault Example Start!  
Press any key to trigger usagefault...  
Coredump Fault Example Rebooted!!!  
Press any key to check coredump reason...  
[Info] Size: 610.  
[Info] Reason: Usagefault caused by an integer divsion by zero!  
Coredump Fault Example End!  

How to debug
===============
1. Download the program and press SW1 to reset the program.
   Following log will be observed in terminal:    
        Coredump Fault Example Start!
        Press any key to trigger usagefault...
2. Attach to running target with debugger.
3. Press any key in terminal.

Update default settings
=================
This example operates based on the default settings (such as the start address and size of the coredump_partition, among others). However, users also have the option to modify these default settings by updating the "coredump_fault.overlay" file or
creating new overlay file. The specific steps are as follows:
1. Update the "coredump_fault.overlay" file.
2. Run the dts_build.py script to generate new "devicetree_generated.h" and "Kconfig.dts_okay" files. The command for running the script is as follows:  
    `python components/gen_hal/scripts/dts_build.py -b frdmrw612 --dtc-overlay-file examples/_boards/frdmrw612/component_examples/coredump_fault/coredump_fault.overlay --output-dir examples/_boards/frdmrw612/component_examples/coredump_fault`
3. Rebuild the example.  
