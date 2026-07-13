# tfm_secureboot_s

## Overview
The Trusted Firmware M (TF-M) PSA Dev API test application provides verification
        that secure service API behaviours are implemented correctly. This is secure
        part of the application.

NOTE: The TF-M main() functions have a non-standard location:
       - Secure main() is in the tf-m\secure_fw\spm\cmsis_psa\main.c file.
       - Non-Secure main() is in the tf-m-tests\app\main_ns.c file.

Workspace structure:
- *tfm_s_crypto_client*: Project creating the static library required by the secure processing environment (S)
- *tfm_secureboot_s*: Project running in the secure processing environment (S)
- *tfm_secureboot_ns*: Project running in the non-secure processing environment (NS)

## Supported Boards
- [FRDM-RW612](../../_boards/frdmrw612/tfm_examples/tfm_secureboot/tfm_secureboot_s/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/tfm_examples/tfm_secureboot/tfm_secureboot_s/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/tfm_examples/tfm_secureboot/tfm_secureboot_s/example_board_readme.md)
- [MCX-W71-EVK](../../_boards/mcxw71evk/tfm_examples/tfm_secureboot/tfm_secureboot_s/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/tfm_examples/tfm_secureboot/tfm_secureboot_s/example_board_readme.md)
- [FRDM-MCXW72](../../_boards/frdmmcxw72/tfm_examples/tfm_secureboot/tfm_secureboot_s/example_board_readme.md)
- [FRDM-MCXW71](../../_boards/frdmmcxw71/tfm_examples/tfm_secureboot/tfm_secureboot_s/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/tfm_examples/tfm_secureboot/tfm_secureboot_s/example_board_readme.md)
