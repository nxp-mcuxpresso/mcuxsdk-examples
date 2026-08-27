Hardware requirements
====================

- USB-C cable
- FRDM-MCXA577 board
- Personal Computer

Board settings
=============

No special settings are required.

Recommended Flash Addresses for CSR operations
=============================================

First, it should be noted that these recommendations are not mandatory, and other addresses within the NXP NS Storage area may also be used. However, using addresses outside the NS Storage region will result in a SecureFault, as the TF-M platform IOCTL service only grants NS access to the designated `NXP_NS_STORAGE` flash region. Please also consult the [Reference Manual](https://www.nxp.com/products) to verify the flash memory map before selecting custom addresses.

On this platform, the NS application communicates with flash through the TF-M platform IOCTL service (`tfm_platform_flash_erase` / `tfm_platform_flash_program`). The NXP NS Storage region is a dedicated flash area placed at the end of flash, and is accessible from the NS world via TF-M.

The table below lists example flash memory addresses within the NXP NS Storage area where the CSR, X.509 certificate, configuration block and status code are stored:

| Module | Start Address | Size |
|--------------|-------------|-------------|
| `NXP NS Storage` | 0x001FC000 | 8 kB * 4 |
| `CSR generation` | 0x001FC000 | CSR_SIZE |
| `X.509 certificate storage` | 0x001FC800 | X509_SIZE |
| `Configuration Block` | 0x001FFF80 | CONFIG_BLOCK_SIZE |
| `APP Status Code` | 0x001FFFFC | 4 |

The CSR_SIZE and X509_SIZE values depend on the specific CSR and certificate implementations used in your application. The CONFIG_BLOCK_SIZE value depends on the specific configuration requirements of your application. However, the maximum allowed CONFIG_BLOCK_SIZE is `124 bytes`.

All CSR and X.509 certificate addresses must be located within the NXP NS Storage region (`0x001FC000` to `0x001FFFFF`). Accessing flash addresses outside this region from the NS world will trigger a SecureFault.
