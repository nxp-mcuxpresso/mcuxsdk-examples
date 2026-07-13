# tfm_psatest_s

## Overview
The Trusted Firmware M (TF-M) PSA Dev API test application provides verification that secure service API behaviours are implemented correctly.
The PSA Dev API tests are the basis for getting the PSA Functional API Certification.

NOTE: The TF-M main() functions have a non-standard location:
       - Secure main() is in the tf-m\secure_fw\spm\cmsis_psa\main.c file.
       - Non-Secure main() is in the tf-m-tests\app\main_ns.c file.

Workspace structure:
- *tfm_s_crypto_client*: Project creating the static library required by the secure processing environment (S)
- *tfm_psatest_s*: Project running in the secure processing environment (S)
- *tfm_psatest_ns*: Project running in the non-secure processing environment (NS)

## Supported Boards
- [FRDM-MCXN236](../../_boards/frdmmcxn236/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [FRDM-RW612](../../_boards/frdmrw612/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [RD-RW612-BGA](../../_boards/rdrw612bga/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [FRDM-MCXA287](../../_boards/frdmmcxa287/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [FRDM-IMXRT700](../../_boards/frdmimxrt700/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [MCIMX93-EVK](../../_boards/mcimx93evk/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [MCXW23-EVK](../../_boards/mcxw23evk/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [MIMXRT700-EVK](../../_boards/mimxrt700evk/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [FRDM-MCXA577](../../_boards/frdmmcxa577/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [FRDM-MCXW23](../../_boards/frdmmcxw23/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [FRDM-MCXW71](../../_boards/frdmmcxw71/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [MCX-W71-EVK](../../_boards/mcxw71evk/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [FRDM-MCXW72](../../_boards/frdmmcxw72/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [FRDM-KW43](../../_boards/frdmkw43/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
- [FRDM-MCXL255](../../_boards/frdmmcxl255/tfm_examples/tfm_psatest/tfm_psatest_s/example_board_readme.md)
