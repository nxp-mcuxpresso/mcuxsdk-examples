Hardware requirements
=====================
- Two Micro USB cable
- EVK-MIMXRT1010 board
- Personal Computer
- Murata CMWC1ZZABR-107-EVB
- LPCXpresso Click Shield

Board settings
==============
Connect the LPCXpresso Click Shield to the board and then connect the Murata Wi-Fi module to mikroBUS slot 2.
Debug console is configured as virtual COM - please connect a micro USB cable between the PC host and USB OTG (J9).

Jumper settings:
J31: open
J32: open


Note:
To debug in qspiflash, following steps are needed:
1. Select the flash target and compile.
2. Set the SW8: 1 off 2 off 3 on 4 off, then power on the board and connect USB cable to J41.
3. Start debugging in IDE.
   - Keil: Click "Download (F8)" to program the image to qspiflash first then clicking "Start/Stop Debug Session (Ctrl+F5)" to start debugging.

Note:
When the USB serial port is emuerated and termial is opened, the printf has been executed, so the message cannot be displayed.
Please use putty,sscom and other terminal that support data caching replace it.

