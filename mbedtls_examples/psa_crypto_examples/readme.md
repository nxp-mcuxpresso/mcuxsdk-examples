# psa_crypto_examples

## Overview

PSA Crypto example to demonstrate cipher operation.

## Supported Boards
- [EVKB-IMXRT1050](../../_boards/evkbimxrt1050/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [EVKB-MIMXRT1060](../../_boards/evkbmimxrt1060/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [EVKC-MIMXRT1060](../../_boards/evkcmimxrt1060/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [EVK-MIMXRT1020](../../_boards/evkmimxrt1020/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [EVK-MIMXRT1024](../../_boards/evkmimxrt1024/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [EVK-MIMXRT1040](../../_boards/evkmimxrt1040/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [EVK-MIMXRT1064](../../_boards/evkmimxrt1064/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [FRDM-MCXA366](../../_boards/frdmmcxa366/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [MCX-W71-EVK](../../_boards/mcxw71evk/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [FRDM-MCXW71](../../_boards/frdmmcxw71/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [FRDM-MCXW23](../../_boards/frdmmcxw23/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- EVK-MIMXRT595
- EVK-MIMXRT685
- MIMXRT685-AUD-EVK
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [FRDM-MCXW72](../../_boards/frdmmcxw72/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [MCX-W72-LOC](../../_boards/mcxw72loc/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [KW47-LOC](../../_boards/kw47loc/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [FRDM-MCXE247](../../_boards/frdmmcxe247/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [FRDM-MCXE31B](../../_boards/frdmmcxe31b/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [FRDM-MCXE32B](../../_boards/frdmmcxe32b/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [FRDM-KW43](../../_boards/frdmkw43/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [FRDM-MCXW70](../../_boards/frdmmcxw70/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- FRDM-IMXRT700
- [FRDM-MCXA266](../../_boards/frdmmcxa266/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- FRDM-MCXL255
- RD-RW612-BGA
- [MIMXRT1160-EVK](../../_boards/evkmimxrt1160/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [FRDM-IMXRT1186](../../_boards/frdmimxrt1186/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- MIMXRT700-EVK
- [FRDM-MCXA577](../../_boards/frdmmcxa577/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [MIMXRT1170-EVKB](../../_boards/evkbmimxrt1170/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [FRDM-IMXRT1152](../../_boards/frdmimxrt1152/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- MCX-N9XX-EVK
- FRDM-RW612
- [FRDM-MCXA287](../../_boards/frdmmcxa287/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- FRDM-MCXN947
- FRDM-MCXN947T
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- FRDM-MCXN236
- MCX-N5XX-EVK
- [FRDM-IMXRT1152](../../_boards/frdmimxrt1152/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [LPCXpresso55S06](../../_boards/lpcxpresso55s06/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [LPCXpresso55S28](../../_boards/lpcxpresso55s28/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [KW43-LOC](../../_boards/kw43loc/mbedtls_examples/psa_crypto_examples/example_board_readme.md)
- [MCXW70-LOC](../../_boards/mcxw70loc/mbedtls_examples/psa_crypto_examples/example_board_readme.md)

## Running the demo
The log below shows the output of the PSA crypto examples in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * PSA crypto example *

cipher encrypt/decrypt AES CBC no padding:
        success!
cipher encrypt/decrypt AES CBC PKCS7 multipart:
        success!
cipher encrypt/decrypt AES CTR multipart:
        success!
cipher encrypt/decrypt AES CBC no padding one go:
        success!
cipher encrypt/decrypt AES CBC PKCS7 padding one go:
        success!
Hash a message SHA-256:
        success!

 * Example End *
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
