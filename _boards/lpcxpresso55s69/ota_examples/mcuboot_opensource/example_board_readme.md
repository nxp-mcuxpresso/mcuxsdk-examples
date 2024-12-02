Hardware requirements
=====================
- LPCXpresso 55S69 board
- Mini/micro USB cable
- Personal Computer

Board settings
============
No special HW settings are required.

In case of MCUBoot for LPC55S69 there is currently no image revert mechanism.
This is due to Flash ECC capability that doesn't allow incremental flash programming.
MCUBoot's revert strategies rely on this mechanism, therefore it's not currently practical
to use any of the MCUBoot's revert strategies like image swapping by move or stratch area.

Current memory layout for LPC55S69 is as follows:
- Bootloader              128kB @ 0x0
- Application (active)    192kB @ 0x20000
- Application (candidate) 192kB @ 0x50000

When signing the image with MCUBoot's imgtool utility, the memory layout described above
defines values of these parameters:
- --slot-size 0x30000
- --max-sectors 400

