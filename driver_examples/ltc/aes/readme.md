# ltc_aes

## Overview
This project is a demonstration program that uses the KSDK software for encryption/decryption sample
data using AES-CBC, AES-CCM, and XCBC-MAC algorithm.

## Running the demo
When the demo runs successfully, from the terminal you can see:

............................. LTC (AES)  DRIVER  EXAMPLE .............................

Testing input string:
          Once upon a midnight dreary, while I pondered weak and weary,
          Over many a quaint and curious volume of forgotten lore,
          While I nodded, nearly napping, suddenly there came a tapping,
          As of some one gently rapping, rapping at my chamber door
          Its some visitor, I muttered, tapping at my chamber door
          Only this, and nothing more.


----------------------------------- AES-CBC method --------------------------------------
AES CBC Encryption of 320 bytes.
AES CBC encryption finished.

AES CBC Decryption of 320 bytes.
AES CBC decryption finished.
Decrypted string :
          Once upon a midnight dreary, while I pondered weak and weary,
          Over many a quaint and curious volume of forgotten lore,
          While I nodded, nearly napping, suddenly there came a tapping,
          As of some one gently rapping, rapping at my chamber door
          Its some visitor, I muttered, tapping at my chamber door
          Only this, and nothing more.


----------------------------------- AES-CCM method --------------------------------------
AES CCM Encryption of  320 bytes.
    using iv length  : 12 bytes
    using aad length : 20 bytes
    using key length : 16 bytes
    using tag length : 8 bytes
AES CCM encryption finished.

AES CCM decryption of 320 bytes.
AES CCM decryption finished.
Decrypted string:
          Once upon a midnight dreary, while I pondered weak and weary,
          Over many a quaint and curious volume of forgotten lore,
          While I nodded, nearly napping, suddenly there came a tapping,
          As of some one gently rapping, rapping at my chamber door
          Its some visitor, I muttered, tapping at my chamber door
          Only this, and nothing more.


----------------------------------- AES-CMAC ------------------------------------------
AES CMAC Computing hash of 320 bytes
Computed hash:
ba fd 2c 65 a 62 7c 18 a0 df b4 20 bb fb 68 27

.............. THE  END  OF  THE  LTC (AES)  DRIVER  EXAMPLE ................................

## Supported Boards
- [EVK-MCIMX7ULP](../../../_boards/evkmcimx7ulp/driver_examples/ltc/aes/example_board_readme.md)
- [FRDM-MCXW71](../../../_boards/frdmmcxw71/driver_examples/ltc/aes/example_board_readme.md)
- [K32W148-EVK](../../../_boards/k32w148evk/driver_examples/ltc/aes/example_board_readme.md)
- [KW45B41Z-EVK](../../../_boards/kw45b41zevk/driver_examples/ltc/aes/example_board_readme.md)
- [KW47-EVK](../../../_boards/kw47evk/driver_examples/ltc/aes/example_board_readme.md)
- [MCX-W72-EVK](../../../_boards/mcxw72evk/driver_examples/ltc/aes/example_board_readme.md)
