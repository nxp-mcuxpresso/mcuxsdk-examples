# tfm_psa_crypto_opaque_key_s

## Overview
This example showcases opaque key handling in TF‑M using PSA Crypto APIs, including key generation, 
wrapped key export, external storage, and re-import. All key material remains protected within 
the secure domain and is never exposed to the non-secure application.

NOTE: The TF-M main() functions have a non-standard location:
       - Secure main() is in the tf-m\secure_fw\spm\cmsis_psa\main.c file.
       - Non-Secure main() is in the tf-m-tests\app\main_ns.c file.

Workspace structure:
- *tfm_s_crypto_client*: Project creating the static library required by the secure processing environment (S)
- *tfm_psa_crypto_opaque_key_s*: Project running in the secure processing environment (S)
- *tfm_psa_crypto_opaque_key_ns*: Project running in the non-secure processing environment (NS)

## Supported Boards
- [FRDM-KW43](../../_boards/frdmkw43/tfm_examples/tfm_psa_crypto_opaque_key/tfm_psa_crypto_opaque_key_s/example_board_readme.md)
