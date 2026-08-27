Hardware requirements
====================

- USB-C cable
- FRDM-MCXA266 board
- Personal Computer

Board settings
=============

No special settings are required.

Recommended Flash Addresses for CSR operations
=============================================

First, it should be noted that these recommendations are not mandatory, and other Flash or RAM address locations may also be used. However, using different locations may result in conflicts with reserved memory addresses, such as those used by littleFS. Please also consult the [Reference Manual](https://www.nxp.com/design/design-center/development-boards-and-designs/FRDM-MCXA266) to check whether other flash sectors are reserved by the platform before selecting custom addresses.

The table below lists example flash memory addresses where the CSR and X.509 certificate can be stored and where the littleFS configuration is located:

| Module | Start Address | Size |
|--------------|-------------|-------------|
| `littleFS` | 0x00066000 | 8 kB * 10 |
| `X.509 certificate storage` | 0x000EA000 | X509_SIZE |
| `CSR generation` | 0x000EC000 | CSR_SIZE |
| `Configuration Block` | 0x000EFF80 | CONFIG_BLOCK_SIZE |
| `APP Status Code` | 0x000EFFFC | 4 |

The CSR_SIZE and X509_SIZE values depend on the specific CSR and certificate implementations used in your application. There is no dedicated memory section for the Configuration Block and the APP status code in the linker files. On this platform it has been decided to use the last `4096 bytes` of the internal flash for this purpose. The CONFIG_BLOCK_SIZE value depends on the specific configuration requirements of your application. However, the maximum allowed CONFIG_BLOCK_SIZE is `124 bytes`. Furthermore, it is important to note that the ITS (Internal Trusted Storage) is managed by littlefs and is located at the same memory address range. This means that the maximum available space for ITS is `8 kB * 10 = 80 kB` including the introduced overhead by the littleFS file system. This value can be increased/decreased by adjusting the `BLOCK_COUNT` parameter in the prj.conf file located at `examples/_boards/frdmmcxa266/el2go_examples/el2go_csr/`

The littleFS memory section must not be overwritten by the CSR or X.509 certificate storage locations. The littleFS configuration can be changed in the prj.conf.
