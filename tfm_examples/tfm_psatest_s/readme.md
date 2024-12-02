# tfm_psatest_s

## Overview
The Trusted Firmware M (TF-M) PSA Dev API test application provides verification that secure service API behaviours are implemented correctly.
The PSA Dev API tests are the basis for getting the PSA Functional API Certification.

NOTE: The TF-M main() functions have a non-standard location:
       - Secure main() is in the tf-m\secure_fw\spm\cmsis_psa\main.c file.
       - Non-Secure main() is in the tf-m-tests\app\main_ns.c file.

## Supported Boards
