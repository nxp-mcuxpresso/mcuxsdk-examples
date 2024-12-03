# tfm_demo_s

## Overview
The Trusted Firmware M (TF-M) demo application provides a sanity check for the TF-M core and RoT services.
It can be used as a template for a new application which requires functionality of the TF-M Secure Services.

NOTE: The TF-M main() functions have a non-standard location:
       - Secure main() is in the tf-m\secure_fw\spm\cmsis_psa\main.c file.
       - Non-Secure main() is in the tf-m-tests\app\main_ns.c file.

## Supported Boards
- FRDM-MCXN947
- [FRDM-RW612](../../_boards/frdmrw612/tfm_examples/tfm_demo/tfm_demo_s/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/tfm_examples/tfm_demo/tfm_demo_s/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/tfm_examples/tfm_demo/tfm_demo_s/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/tfm_examples/tfm_demo/tfm_demo_s/example_board_readme.md)
