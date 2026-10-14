# psa_crypto_mldsa

## Overview

PSA Crypto example to demonstrate ML-DSA operations. It showcases key generation,
key imports, exports, message signing and signature verification.

## Supported Boards
- [FRDM-MCXA577](../../_boards/frdmmcxa577/mbedtls_examples/psa_crypto_mldsa/example_board_readme.md)

## Running the demo
The log below shows the output of the example in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * PSA ML-DSA example *

******** ML-DSA-44 ********
Call psa_import_key() OK
Call psa_sign_message() OK
Call psa_verify_message() OK
Call psa_export_key() OK
Key pair matches original seed value OK
Call psa_export_public_key() OK
Public key matches expected value OK
Call psa_destroy_key() OK
******** ML-DSA-44 (Public Key) ********
Call psa_import_key() with key pair OK
Call psa_sign_message() with key pair OK
Call psa_export_public_key() with key pair OK
Call psa_destroy_key() with key pair OK
Call psa_import_key() with public key OK
Call psa_verify_message() with public key OK
Call psa_export_public_key() with public key OK
Public key matches expected value OK
Call psa_destroy_key() with public key OK
******** ML-DSA-44 (Invalid Key Pair Size) ********
Call psa_import_key() with too large key pair size OK
******** ML-DSA-65 ********
Call psa_generate_key() OK
Call psa_sign_message() OK
Call psa_verify_message() OK
Call psa_destroy_key() OK
******** ML-DSA-65 (Invalid Signature) ********
Call psa_generate_key() OK
Call psa_sign_message() OK
Call psa_verify_message() on invalid signature OK
Call psa_destroy_key() OK
******** ML-DSA-87 ********
Call psa_generate_key() OK
Call psa_sign_message() OK
Call psa_verify_message() OK
Call psa_destroy_key() OK
******** ML-DSA-87 (Small Signature Buffer Size) ********
Call psa_generate_key() OK
Call psa_sign_message() with small output signature buffer OK
Call psa_destroy_key() OK

 * Example End *
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
