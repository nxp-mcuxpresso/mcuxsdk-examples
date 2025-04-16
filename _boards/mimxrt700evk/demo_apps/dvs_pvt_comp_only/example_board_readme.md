<!--
 * @Author: Chay Guo changyi.guo@nxp.com
 * @Date: 2025-04-10 12:52:36
 * @LastEditors: Chay Guo changyi.guo@nxp.com
 * @LastEditTime: 2025-04-18 12:58:04
 * @FilePath: \sdk-next\mcuxsdk\examples\_boards\mimxrt700evk\demo_apps\dvs_pvt_comp_only\example_board_readme.md
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
-->
Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Board settings
============
Make sure JP7(2-3) connected for 3.3V supply for VDDIO_2. 
JP55(1-2) disconnected to ommit the impact of MCU-LINK during DPD and FDPD mode.

Make sure the board is supplied by PMIC,
   Jumper setting - JP1(1-2), JP2(2-3), JP3(1-2), JP4(1-2).

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the MCU-LINK on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Build cm33_core1 project first, then the cm33_core0 project.
4.  Download the core0 program to the target board.
5.  Either press the reset button on your board or launch the debugger in your IDE to
    begin running the demo.

Running the demo
================
The log below shows the output of the demo in the cm33_core0 terminal window:
```
PVT Application Demo

This application uses the PMIC for power supply.
SILICON_REV_ID = A0
Build Time: Apr 15 2025--17:50:38 
PVTS delay = 16
Core Clock: 192000000Hz 
Input any key to start
```

The cm33_core1 terminal window shows the below log,
```
####################  Power Mode Demo - CPU1 ####################

    Build Time: Feb  4 2024--13:36:32 
    Core Clock: 32000000Hz 
Entering Deep sleep...
```
