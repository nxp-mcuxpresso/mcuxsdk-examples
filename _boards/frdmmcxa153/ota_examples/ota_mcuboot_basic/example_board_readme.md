Hardware requirements
=====================
- FRDM-MCXA153 board
- USB-C cable
- Personal Computer

Board settings
==============
No special HW settings.

Board specific example notes
----------------------------
In case of this board MCUBoot si configured for `OVERWRITE_ONLY` image strategy.
This means that an image in the secondary slot is simply moved to the primary slot
by overwriting the current version. This means that it's not possible to revert
back to previous version once the new image is installed.

