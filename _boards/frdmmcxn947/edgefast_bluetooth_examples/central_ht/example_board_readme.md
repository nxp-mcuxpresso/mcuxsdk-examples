Hardware requirements
=====================
- Type-C USB cable
- FRDM-MCXN947 board
- Personal Computer
one of the following modules:
  - FRDM-IW416-AW-AM510 Board
  - An adapter board(X-FRDM-WIFI-M.2) and Embedded Artists 2LL M.2 Module (EAR00500) - direct M2 connection.

Board settings
==============
Set the hardware jumpers to default settings.
If you want to use Embedded Artists 2LL M.2 Module(EAR00500), please change the macro to WIFI_IW610_BOARD_MURATA_2LL_M2.

Hardware rework guide:
======================
The hardware should be reworked according to the Hardware Rework Guide for FRDM-MCXN947 and X-FRDM-WIFI-M.2 Adapter in document Hardware Rework Guide for EdgeFast BT PAL.
The hardware should be reworked according to the Hardware Rework Guide for FRDM-MCXN947 with FRDM-IW416-AW-AM510 in document Hardware Rework Guide for EdgeFast BT PAL.

FRDM-IW416-AW-AM510 Board settings
==============================
FRDM-IW416-AW-AM510 user guide: https://www.azurewave.com/img/nxp/FRDM-IW416-AW-AM510_user%20guide_20240919.pdf
FRDM-IW416-AW-AM510 datasheet: https://www.azurewave.com/img/nxp/FRDM-IW416-AW-AM510_DS_DF_A_STD.pdf
AW-AM510 datasheet: https://www.azurewave.com/img/nxp/AW-AM510_DS_DF_F_STD.pdf
IW416 datasheet: https://www.nxp.com.cn/docs/en/data-sheet/IW416.pdf

The Adapter Board(X-FRDM-WIFI-M.2) settings
==============================
Connect the X-FRDM-WIFI-M.2 to FRDM-MCXN947 by J1, J2, J3, J4.
BT_CTS(On X-FRDM-WIFI-M.2) - J9 PIN26 (On FRDM-MCXN947)
BT_RTS(On X-FRDM-WIFI-M.2) - J8 PIN6 (On FRDM-MCXN947)
BT_RXD(On X-FRDM-WIFI-M.2 which connect to Embedded Artists 2LL M.2 Module) - Opposite BT_TXD pin (On X-FRDM-WIFI-M.2 which connect to FRDM-MCXN947)
BT_TXD(On X-FRDM-WIFI-M.2 which connect to Embedded Artists 2LL M.2 Module) - Opposite BT_RXD pin (On X-FRDM-WIFI-M.2 which connect to FRDM-MCXN947)
J8(On X-FRDM-WIFI-M.2) - 1.8V
J24(On X-FRDM-WIFI-M.2) - 3.3V
J19(On X-FRDM-WIFI-M.2) - 1.8V
J25(On X-FRDM-WIFI-M.2) - 3.3V
J15(On X-FRDM-WIFI-M.2) - 1.8V
J16(On X-FRDM-WIFI-M.2) - 3.3V
J17(On X-FRDM-WIFI-M.2) - 1.8V
J18(On X-FRDM-WIFI-M.2) - 3.3V
SDIO_RST(On X-FRDM-WIFI-M.2) - Opposite SDIO_RST pin(On X-FRDM-WIFI-M.2)

Murata Solution Board settings
==============================
Embedded Artists 2LL module datasheet: https://www.embeddedartists.com/wp-content/uploads/2024/12/2LL_M2_Datasheet.pdf

Note:
=====
Whole memory must be erased before this demo is flashed.
After downloaded binary into Quad SPI Flash and boot from Quad SPI Flash directly,
please reset the board by pressing SW1 or power off and on the board to run the application.

====
If you want to get HCI log , please define macro CONGIF_BT_SNOOP to 1 in edgefast_bluetooth_app.h, then connect the OTG with U-disk to the J11.
You will get a file named btsnoop on the U-disk.You can change its extension to .cfa , then open it with ComProbe Protocol Analysis System to view the HCI logs.
