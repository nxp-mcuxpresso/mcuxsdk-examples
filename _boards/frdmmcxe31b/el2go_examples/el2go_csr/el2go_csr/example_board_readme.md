Hardware requirements
====================

- USB-C cable
- FRDM-MCXE31B board
- Personal Computer

Board settings
=============

No special settings are required.

Recommended Flash Addresses for CSR operations
=============================================

First, it should be noted that these recommendations are not mandatory, and other Flash or RAM address locations may also be used. However, using different locations may result in conflicts with *reserved* memory addresses, such as those used by littleFS or the HSEB firmware.

The table below lists example data flash memory addresses where the CSR and X.509 certificate can be stored:

| Operation | Start Address | End Address |
|--------------|-----------|-------------|
| `CSR generation` | 0x10014800 | 0x10014FFF | 
| `X.509 certificate storage` | 0x10015000 | 0x10015F7F | 

A dedicated memory section has been created for the configuration block and the application status code, which can be seen in the linker files located at [examples/_boards/frdmmcxe31b/el2go_examples/el2go_csr/linker/](../../../../ examples/_boards/frdmmcxe31b/el2go_examples/el2go_csr/linker/).

For the sake of completeness, the addresses where the littleFS configuration and HSEB firmware are stored are listed below. These memory sections must not be overwritten by the CSR or X.509 certificate storage locations. The littleFS size can be taken from the prj.conf file located at [examples/_boards/frdmmcxe31b/el2go_examples/el2go_csr/](../../../../examples/_boards/frdmmcxe31b/el2go_examples/el2go_csr/).

| Module | Start Address | Size |
|--------------|-----------|-------------|
| `littleFS` | 0x10000000 | BLOCK_SIZE x BLOCK_COUNT | 
| `HSEB FW` | 0x10016000 | 0x1001FFFF |

