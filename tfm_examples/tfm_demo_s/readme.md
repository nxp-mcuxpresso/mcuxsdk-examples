# tfm_demo_s

## Overview
The Trusted Firmware M (TF-M) demo application provides a sanity check for the TF-M core and RoT services.
It can be used as a template for a new application which requires functionality of the TF-M Secure Services.

NOTE: The TF-M main() functions have a non-standard location:
       - Secure main() is in the tf-m\secure_fw\spm\cmsis_psa\main.c file.
       - Non-Secure main() is in the tf-m-tests\app\main_ns.c file.

## Supported Boards
