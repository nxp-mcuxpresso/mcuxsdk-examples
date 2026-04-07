Hardware requirements
=====================
- Micro USB cable
- IMX95LPD5EVK-19 board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer
- DUAL LVDS panel
- AP1302 sensor

Board settings
==============
Transfer data from one board instance to another board's instance.
SPI3 pins are connected with SPI3 pins of another board
Note:
1. the ap1302 is an external isp, it need special firmware from OnSemi GitHub.
   pls download ap1302_60fps_ar0144_27M_2Lane_awb_tuning.bin from: https://github.com/ONSemiconductor/ap1302_binaries/tree/main/NXP_i.MX93
2. You can use Jlink or other method to load fw into memory.
   such as Jlink tool: 'connect' then select 'MIMX9596_M7' device, 'S' select SWD.
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
