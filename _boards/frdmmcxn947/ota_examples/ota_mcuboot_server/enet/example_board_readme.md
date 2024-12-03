Hardware requirements
=====================
- Mini/micro USB cable
- RJ45 Network cable
- FRDM-MCXN947 board
- Personal Computer

Board settings
============
Note that when HW flash swapping feature is used the images are swapped so that address
range of the primary image translates to the range of the secondary image and vice versa.
The web page of the ota_mcuboot_server example shows image state as read from the bus
so when HW flash swapping is used it will not match with the physical image state - the slots
will be swapped.
