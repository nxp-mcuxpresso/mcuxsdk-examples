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
