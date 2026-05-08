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

The table below lists example flash memory addresses where the CSR and X.509 certificate can be stored and where the littleFS configuration and HSEB firmware are located:

| Module |  Start Address | Size |
|--------------|-------------|
| `Configuration Block` | 0x007D2000 | CONFIG_BLOCK_SIZE |
| `CSR generation` | 0x007D3000 | CSR_SIZE |
| `X.509 certificate storage` | 0x00600000 | X509_SIZE |  
| `littleFS` | 0x10000000 | BLOCK_SIZE x BLOCK_COUNT | 
| `HSEB FW` | 0x10016000 | 0x9FFF |

The `CSR_SIZE` and `X509_SIZE` values depend on the specific CSR and certificate implementations used in your application. A dedicated memory section has been created for the `Configuration Block` and the application status code, which can be seen in the linker files located at [examples/_boards/frdmmcxe31b/el2go_examples/el2go_csr/linker/](../../../../ examples/_boards/frdmmcxe31b/el2go_examples/el2go_csr/linker/). The `CONFIG_BLOCK_SIZE` value depends on the specific configuration requirements of your application. The application status code will be written at the last 4 bytes of the configuration block memory section.

The littleFS and HSEB FW memory sections must not be overwritten by the CSR or X.509 certificate storage locations. The littleFS size can be taken from the prj.conf file located at [examples/_boards/frdmmcxe31b/el2go_examples/el2go_csr/](../../../../examples/_boards/frdmmcxe31b/el2go_examples/el2go_csr/) and the information about HSEB FW size from the [Reference Manual](https://www.nxp.com/products/MCX-E31).


