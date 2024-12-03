# tfm_regression_s

## Overview
The Trusted Firmware M (TF-M) regression test application provides testing of the TF-M core and its RoT services:
 - Secure Storage Service
 - Attestation Service
 - Crypto Service
 - TF-M Audit Log Service
 - Platform Service
 
NOTE: The TF-M main() functions have a non-standard location:
       - Secure main() is in the tf-m\secure_fw\spm\cmsis_psa\main.c file.
       - Non-Secure main() is in the tf-m-tests\app\main_ns.c file.

## Supported Boards
- [FRDM-MCXN947](../../_boards/frdmmcxn947/tfm_examples/tfm_regression/tfm_regression_s/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/tfm_examples/tfm_regression/tfm_regression_s/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/tfm_examples/tfm_regression/tfm_regression_s/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/tfm_examples/tfm_regression/tfm_regression_s/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/tfm_examples/tfm_regression/tfm_regression_s/example_board_readme.md)
