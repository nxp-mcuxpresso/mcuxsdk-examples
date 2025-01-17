Hardware requirements
=====================
- Micro USB cable
- evkmimxrt1180 board
- Personal Computer
- One of the following modules:
  - Embedded Artists 1XK M.2 Module (EAR00385) - direct M2 connection.
  - Embedded Artists 1ZM M.2 Module (EAR00364) - direct M2 connection.
  - Embedded Artists 2EL M.2 Module (Rev-A1) - direct M2 connection.

Board settings
==============
Before building the example application select Wi-Fi module macro in the app_bluetooth_config.h. (see #define WIFI_<SoC Name>_BOARD_<Module Name>).
If you want to use Embedded Artists Type 1XK module(EAR00385 M2 only), please change the macro to WIFI_IW416_BOARD_MURATA_1XK_M2.
If you want to use Embedded Artists Type 1ZM module(EAR00364 M2 only), please change the macro to WIFI_88W8987_BOARD_MURATA_1ZM_M2.
If you want to use Embedded Artists Type 2EL module(Rev-A1 M2 only), please change the macro to WIFI_IW612_BOARD_MURATA_2EL_M2.

Jumper settings for RT1180:
===========================
connect J1 1-2
connect J2 with external power(controlled by SW1)
connect J76 2-3
connect J57 2-3

Hardware rework guide:
======================
The hardware should be reworked according to the hardware rework guide for evkmimxrt1180 and Murata 1XK M.2 Adapter in document Hardware Rework Guide for EdgeFast BT PAL.
The hardware should be reworked according to the hardware rework guide for evkmimxrt1180 and Murata 1ZM M.2 Adapter in document Hardware Rework Guide for EdgeFast BT PAL.
The rework guide for evkmimxrt1180 and Murata 2EL M.2 Adapter is same as evkmimxrt1180 and Murata 1ZM M.2 Adapter.

Murata Solution Board settings
==============================
Murata uSD-M.2 adapter resource page: https://www.murata.com/en-us/products/connectivitymodule/wi-fi-bluetooth/overview/lineup/usd-m2-adapter
Murata uSD-M.2 adapter datasheet: https://www.murata.com/-/media/webrenewal/products/connectivitymodule/asset/pub/rfm/data/usd-m2_revb1.ashx
Embedded Artists M.2 module resource page: https://www.embeddedartists.com/m2
Embedded Artists 1XK module datasheet: https://www.embeddedartists.com/doc/ds/1XK_M2_Datasheet.pdf
Embedded Artists 1ZM module datasheet: https://www.embeddedartists.com/doc/ds/1ZM_M2_Datasheet.pdf
Embedded Artists 2EL module datasheet: https://www.embeddedartists.com/doc/ds/2EL_M2_Datasheet.pdf

Note:
=====
To ensure that the LITTLEFS flash region has been cleaned,
all flash sectors need to be erased before downloading example code.
For Peripheral DMA to be used, the application core must first gain TRDC access from edgelock FW, so this demo
must be runnning with edgelock FW alive.
Whole memory must be erased before this demo is flashed.
After downloaded binary into qspiflash and boot from qspiflash directly,
please reset the board by pressing SW4 or power off and on the board to run the application.
