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

First, it should be noted that these recommendations are not mandatory, and other Flash or RAM address locations may also be used. However, using different locations may result in conflicts with reserved memory addresses, such as those used by littleFS or the HSEB firmware.

The table below lists example flash memory addresses where the CSR and X.509 certificate can be stored and where the littleFS configuration and HSEB firmware are located:

| Module | Start Address | Size | 
|--------------|-------------| -------------| 
| `littleFS` | 0x10000000 | 65536  | 
| `CSR generation` | 0x10014800 | CSR_SIZE |
| `X.509 certificate storage` | 0x10015000 | X509_SIZE |
| `Configuration Block` | 0x10015F80 | CONFIG_BLOCK_SIZE |
| `APP Status Code` | 0x10015FFC | 4 |
| `HSEB FW` | 0x10016000 | 40959 |

The CSR_SIZE and X509_SIZE values depend on the specific CSR and certificate implementations used in your application. There is no dedicated memory section for the Configuration Block and the APP status code in the linker files. On this platform it has been decided to use the last `4096 bytes` of the internal data flash for this purpose. The CONFIG_BLOCK_SIZE value depends on the specific configuration requirements of your application. However, the maximum allowed CONFIG_BLOCK_SIZE is `124 bytes`. Furthermore, it is important to note that the ITS (Internal Trusted Storage) is managed by littlefs and is located at the same memory address range. This means that the maximum available space for ITS is `65536 bytes` including the introduced overhead by the littleFS file system. This leaves `18433 bytes` of data flash available for further application specific usage. This value can be increased/decreased by adjusting the `BLOCK_COUNT` parameter in the prj.conf file located at [examples/_boards/frdmmcxe31b/el2go_examples/el2go_csr/](../../../../examples/_boards/frdmmcxe31b/el2go_examples/el2go_csr/)

The littleFS and HSEB FW memory sections must not be overwritten by the CSR or X.509 certificate storage locations. The littleFS can be changed in the prj.conf. The information about HSEB FW size can be taken from the [Reference Manual](https://www.nxp.com/products/MCX-E31).