Hardware requirements
=====================
- micro USB cable
- LPCXpresso55S69 board
- Personal Computer

Board settings
==============
No special HW settings.

Board specific example notes
----------------------------
LPC55s69 has internal flash that implements data integrity protection
by ECC. One of the main implications is that once page is erased it
cannot be read unless it's programmed. Reading unprogrammed page results
in an exception. Because of this feature the example output can give
warnings about failing to read flash. For example when running `image erase`
command and then `image info` the secondary slot will contain unprogrammed
pages which will be reflected in the image info output by a failed-read message.

In case of this board MCUBoot si configured for `OVERWRITE_ONLY` image strategy.
This means that an image in the secondary slot is simply moved to the primary slot
by overwriting the current version. This means that it's not possible to revert
back to previous version once the new image is installed.
MCUBoot's image swapping modes are not possible to use because of the ECC mechanism
of LPC55s69. Mainly due to the fact that flash sector size is equal to flash page
size (512 bytes) and it's not possible to program the page multiple times after
it's erased (incremental write). Since MCUBoot is not compatible with these features
the image swapping strategies cannot be used.


