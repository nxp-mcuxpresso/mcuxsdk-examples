Hardware requirements
=====================
- Micro USB cable
- IMX95LP4XEVK-15 board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer
- AP1302 sensor

Board settings
==============
For MIPI2HDMI card(1920*1080), connect MIPI_DSICSI port to MIPI2HDMI card(ADV7535) card.
For AP1302 sensor with MINISAS connect to MIPI-CSI(MINI SAS).
Prepare the Demo
================
1.  Connect 12V power supply and J-Link Debug Probe to the board, switch SW4 to power on the board.
2.  Connect a micro USB cable between the host PC and the J31 USB port on the cpu board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the program to the target board.
5.  Launch the debugger in your IDE to begin running the example.

Note:
1. the ap1302 is an external isp, it need special firmware from OnSemi GitHub.
   pls download ap1302_60fps_ar0144_27M_2Lane_awb_tuning.bin from: https://github.com/ONSemiconductor/ap1302_binaries/tree/main/NXP_i.MX93
2. You can use Jlink or other method to load fw into memory.
   such as Jlink tool: 'connect' then select 'MIMX9594_M7' device, 'S' select SWD.
   loadbin "C:\***\ap1302_60fps_ar0144_27M_2Lane_awb_tuning.bin" 0x87000000
================
Running the demo
================
ISI MIPI-CSI2 example start...
Please load camera firmware into memory:87000000, input 'A' in console after finished load
FW Load into memory done
CSI interface init pass.
DISPLAY Device init pass.
When the example runs successfully, you can see the expected figure shown in screen.
