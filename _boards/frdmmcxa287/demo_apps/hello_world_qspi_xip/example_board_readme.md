Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXA287 board
- Personal Computer

Software requirements
=====================
- BLHost 2.6.7 or later

Board settings
==============

Prepare the Demo
================
1.  Connect a type-c USB cable between the PC host and the MCU-Link USB port on the board
2.  Determine the COM port number (See Appendix A in Getting started guide for description how to determine serial port number).
3.  Open a command prompt and navigate to the BLHost executables folder (<BLHost root>\bin\<OS environment>).
4.  Configure the EFLASH_BOOTEN in CMPA using blhost.
    4.1  Mass erase IFR
         4.1.1 Update MCU-Link firmware to Jlink with LinkServer
         4.1.2 Run Debug mailbox mass erase command:
            # JLink.exe -device MCXA577 -if SWD -speed 4000 -CommandFile debugmailboxerase.jlink
            or launch the erasecmpa.bat
            Note: "mem8 0x11002200 1" should output "11002200 = FF", if not, please disconnect and reconnect MCU-Link USB and try again.
    4.2  Enter ISP mode so that the blhost could program the CMPA.
            Press and hold SW3(ISP key) => Press and release SW1 => Release SW3
         Another way to enter ISP mode.
            # nxpdebugmbox.exe -i pyocd ispmode -m 0
    4.3  Program IFR with bootfromflexspi.bin
            # blhost -p COMxx -- flash-erase-region 0x11002000 0x400
            # blhost -p COMxx write-memory 0x11002000 bootfromflexspi.bin
            # blhost -p comxx read-memory 0x11002200 1
             or launch the bootfromflexspi.bat
          Note: "xx" in the command above should be replaced with the COM port number of your FRDM.
                "blhost -p COMxx read-memory 0x11002200 1" should output 04, please disconnect and reconnect MCU-Link USB and try again.
5.  After successfully running the script above, press and release the reset button. 
6.  Open a serial terminal with the following settings :
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
7.  Download the program to the target board.
8.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.
9.  The file bootfromflash.bin is used to reset the boot source to internal flash, follow step 4 to program the bootfromflash.bin to 0x11002000

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
hello world.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
