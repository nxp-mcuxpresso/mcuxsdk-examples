Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN947 board
- Personal Computer
- FRDM-IW416-AW-AM510 Board

Board settings
==============
Set the hardware jumpers to default settings.

Hardware rework guide:
======================
The hardware should be reworked according to the Hardware Rework Guide for FRDM-MCXN947 with FRDM-IW416-AW-AM510 in document Hardware Rework Guide for EdgeFast BT PAL.

FRDM-IW416-AW-AM510 Board settings
==============================
FRDM-IW416-AW-AM510 user guide: https://www.azurewave.com/img/nxp/FRDM-IW416-AW-AM510_user%20guide_20240919.pdf
FRDM-IW416-AW-AM510 datasheet: https://www.azurewave.com/img/nxp/FRDM-IW416-AW-AM510_DS_DF_A_STD.pdf
AW-AM510 datasheet: https://www.azurewave.com/img/nxp/AW-AM510_DS_DF_F_STD.pdf
IW416 datasheet: https://www.nxp.com.cn/docs/en/data-sheet/IW416.pdf

Note:
=====
Whole memory must be erased before this demo is flashed.
After downloaded binary into Quad SPI Flash and boot from Quad SPI Flash directly,
please reset the board by pressing SW1 or power off and on the board to run the application.

====
If you want to get HCI log , please define macro CONGIF_BT_SNOOP to 1 in edgefast_bluetooth_app.h, then connect the OTG with U-disk to the J11.
You will get a file named btsnoop on the U-disk.You can change its extension to .cfa , then open it with ComProbe Protocol Analysis System to view the HCI logs.
