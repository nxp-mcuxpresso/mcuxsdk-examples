# ecat_foe

## 1. Overview

This example shows how to implement EtherCAT FoE (File Access over EtherCAT) on top of the
Beckhoff Slave Stack Code (SSC) and use it to perform a firmware update through MCUboot.

| Item | Value |
|------|-------|
| Mailbox protocol | FoE |
| Bootloader | MCUboot (`SWAP_MOVE` strategy) |
| Image signing | ECDSA-P256 |
| FoE password | 12369874 |
| MainDevice used for the demo | TwinCAT 3 |

> **Important:** the project cannot be built as shipped. The SSC source code must be
> generated first - see [2. Build preparation](#2-build-preparation).

## 2. Build preparation

1. Generate the SSC source code: [Generate SSC Code for FoE](../topics/generate_ssc_for_foe.md)
2. Build `mcuboot_opensource` and the FoE example, then sign and flash both images - the exact
   addresses, boot switch settings and signing arguments are board specific, see the board
   readme listed in [5. Supported boards](#5-supported-boards).

## 3. Expected output

After a successful FoE upgrade the serial console prints:

```
System reset...
hello sbl.
Bootloader Version 1.9.0
Primary image: magic=good, swap_type=0x1, copy_done=0x3, image_ok=0x1
Secondary image: magic=good, swap_type=0x1, copy_done=0x3, image_ok=0x3
Boot source: none
Swap type: test
Starting swap using move algorithm.
Bootloader chainload address offset: 0x40000
Reset_Handler address offset: 0x40400
Jumping to the image

Start the SSC FoE example...
Image version: 2.3.0
Image state: testing
Set image as confirmed: success
```

## 4. TwinCAT setup

TwinCAT 3 is used as the EtherCAT MainDevice. Perform the steps in this order:

| Step | Document |
|------|----------|
| 1. Generate SSC code | [Generate SSC Code for FoE](../topics/generate_ssc_for_foe.md) |
| 2. Install TwinCAT | [TwinCAT Installation](../topics/download_and_install_the_twincat_software.md) |
| 3. Write the ESI file to EEPROM | [TwinCAT EEPROM Update](../topics/twincat_eeprom_update_foe.md) |
| 4. Create the project and run the FoE upgrade | [TwinCAT Project Setup](../topics/twincat_project_setup_foe.md) |

Additional reading: [Note about the source code in the document](../topics/note_about_the_source_code_in_the_document.md)

## 5. Supported boards

| Board | Board specific readme |
|-------|-----------------------|
| MIMXRT1180-EVK | [example_board_readme.md](../../_boards/evkmimxrt1180/ecat_examples/foe/example_board_readme.md) |
| FRDM-MIMXRT1186 | [example_board_readme.md](../../_boards/frdmimxrt1186/ecat_examples/foe/example_board_readme.md) |

## 6. Notes

* Every new image must carry a version number higher than the currently installed one.
* For more details about the bootloader, refer to the `mcuboot_opensource` example.
