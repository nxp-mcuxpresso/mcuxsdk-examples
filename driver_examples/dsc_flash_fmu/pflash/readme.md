# dsc_pflash_fmu

## Overview
The pflash example shows how to use flash driver to operate program flash:


## Running the demo
When the example runs successfully, you can see the similar information from the terminal as below.

    PFlash Example Start

    Flash Information for Flash 0 Array:
     Program Flash block bass address:      4096, Hex: (0x400000)
     Program Flash block Size:              512 KB, Hex: (0x80000)
     Program Flash block Sector Size:       8 KB, Hex: (0x2000)
    Flash Information for Flash 1 Array:
     Program Flash block bass address:      4608, Hex: (0x480000)

    Erase a sector of flash at address 4fe000

    Sector content:
    004fe000  00 00 00 00 01 00 00 00 02 00 00 00 03 00 00 00 |................|
    004fe010  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|
    004fe020  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|
    004fe030  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|

    Verify the sector is already erased: NOT_ERASED
    Erasing the sector...
    Sector content after erase:
    004fe000  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|
    004fe010  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|
    004fe020  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|
    004fe030  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|

    Verify the sector has been erased: ERASED
    Successfully erased sector 0x4fe000 -> 0x500000

    Program a buffer to a phrase of flash

    Buffer content:
    00000740  00 00 00 00 01 00 00 00 02 00 00 00 03 00 00 00 |................|

    Flash content after programming:
    004fe000  00 00 00 00 01 00 00 00 02 00 00 00 03 00 00 00 |................|
    004fe010  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|
    004fe020  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|
    004fe030  FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF FF |................|

    Successfully Programmed and Verified Location 0x4fe000 -> 0x4fe010

    Test erase verify:
     Programmed SECTOR:     NOT_ERASED
     Programmed PAGE:       NOT_ERASED
     Programmed PHRASE:     NOT_ERASED
     Following PAGE:        ERASED
     Following PHRASE:      ERASED

    End of PFlash Example


## Supported Boards
- mc56f85xxxevk
