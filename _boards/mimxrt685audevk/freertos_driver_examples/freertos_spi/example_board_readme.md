Hardware requirements
=====================
- Micro USB cable
- MIMXRT685-AUD-EVK board
- Personal Computer

Board settings
==============
Connect SPI5 pins to SPI4 pins:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
INSTANCE0(SPI5)     CONNECTS TO         INSTANCE1(SPI4)
Pin Name   Board Location     Pin Name  Board Location
MISO       J28 pin 5          MISO      J27 pin 2
MOSI       J28 pin 4          MOSI      J27 pin 1
SCK        J28 pin 6          SCK       J27 pin 3
PCS0       J28 pin 3          PCS1      J27 pin 5
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Make sure the JP41 2-3 ,JP42 2-3 are connected. 