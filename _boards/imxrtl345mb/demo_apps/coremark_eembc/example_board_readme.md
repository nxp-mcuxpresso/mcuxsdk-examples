Hardware requirements
=====================
- Type-C USB cable
- IMXRTL345MB board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a USB Type-C cable between the host PC and the MCU-Link USB port on the target board.
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Download the program to the target board.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
===============
The log below shows the output of the EEMBC coremark in the terminal window:

Trigger Coremark...
2K performance run parameters for coremark.
CoreMark Size : 666
Total ticks : 233241818
Total time (secs): 11.662091
Iterations/Sec : 85.747917
Iterations : 1000
Compiler version : ARMClang -D6.23.0
Compiler flags : -include /workspace/mcuxsdk/build/css/mcux_config.h -include /workspace/mcuxsdk/build/css/mcuxsdk_version.h -mcpu=cortex-m55 -mfpu=fpv5-sp-d16 -std=c99 -fshort-enums -fshort-wchar -ffunction-sections -fno-common -fdata-sections -fno-builtin -mthumb -fno-rtti -Werror -Omax
Memory location : STATIC
seedcrc : 0xe9f5
[0]crclist : 0xe714
[0]crcmatrix : 0x1fd7
[0]crcstate : 0x8e3a
[0]crcfinal : 0xd340
Correct operation validated.
See README.md for run and reporting rules.
CoreMark 1.0 : 85.747917 / ARMClang -D6.23.0 -include /workspace/mcuxsdk/build/css/mcux_config.h -include /workspace/mcuxsdk/build/css/mcuxsdk_version.h -mcpu=cortex-m55 -mfpu=fpv5-sp-d16 -std=c99 -fshort-enums -fshort-wchar -ffunction-sections -fno-common -fdata-sections -fno-builtin -mthumb -fno-rtti -Werror -Omax / STATIC
