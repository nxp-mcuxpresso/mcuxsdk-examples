# tfm_secureboot_s

## Overview
The Trusted Firmware M (TF-M) PSA Dev API test application provides verification
        that secure service API behaviours are implemented correctly. This is secure
        part of the application.

NOTE: The TF-M main() functions have a non-standard location:
       - Secure main() is in the tf-m\secure_fw\spm\cmsis_psa\main.c file.
       - Non-Secure main() is in the tf-m-tests\app\main_ns.c file.

## Supported Boards
- [FRDM-RW612](../../_boards/frdmrw612/tfm_examples/tfm_secureboot/tfm_secureboot_s/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/tfm_examples/tfm_secureboot/tfm_secureboot_s/example_board_readme.md)
