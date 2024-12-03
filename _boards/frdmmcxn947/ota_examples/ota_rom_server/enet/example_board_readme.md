Hardware requirements
=====================
- Mini/micro USB cable
- RJ45 Network cable
- FRDM-MCXN947 board
- Personal Computer

Board settings
============


Board provisioning details
--------------------------
For board provisioning it's best to use MCUXpresso Secure Provisioning tool.
Besides required setup the following settings need to be applied:
    - Boot type set as "Signed"
    - `CFPA_PAGE_VERSION` set to at least 1 (non-zero and non-all-one)
    - Flash swapping enabled; CMPA field `FLASH_CFG.FLASH_REMAP_SIZE` set to 0b11111

The Dual image boot option allows to select for which slot (slot0/slot1) the SB3 file
is being created. To create SB3 file used for an update, the target image should be set
as "Image 1" - this reflects the process of using the second dormant slot as a space
for the new image. It's important to realize that with Flash Swapping enabled the ROM
selects an image with higher Image version. If higher version image is located in
the second slot (offset 0x100000) it swaps the slot0/slot1 addresses making it effectively
appear at the beginning of the flash (offset 0). Therefore also make sure that Image version
of the new image is higher than the currently installed one.

