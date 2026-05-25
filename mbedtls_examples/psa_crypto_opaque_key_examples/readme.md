# psa_crypto_opaque_key_examples

## Overview

The PSA crypto opaque key example illustrates the generation and usage of keys with different attributes using PSA API's

The PSA Key Attributes consist of :

PSA Key Identifier
PSA Key Lifetime
PSA Key Type
PSA Key Policies composed of PSA Key Usage and PSA Key Algorithm

PSA Key Lifetime is composed of a location indicator and a persistence level.
The location indicator identifies whethere the application generates the key in local storage
i.e transparent key or as an opaque key in SoC specific NXP secure subsystem.

The location of the key is provided as a Kconfig option for the example (TEST_KEY_LOCATION)
For RT1180, default location chosen in example Kconfig is 0x1 (PSA_CRYPTO_ELE_S4XX_LOCATION).
The keys generated in the example for RT1180 are generated within ELE enclave's  key store.
For MCXN and RW612 based devices the location for opaque keys is 0xc00401 which indicates the
keys are generated within ELS50 subsystem keyslots and never leave the ELS50 in plain format.

The example also demonstrates generation of keys with different persistence levels. Persistent keys
may be demonstrated if MBEDTLS_PSA_CRYPTO_STORAGE_C is enabled in the config file.

## Supported Boards
- [MCX-W71-EVK](../../_boards/mcxw71evk/mbedtls_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
- [FRDM-MCXW71](../../_boards/frdmmcxw71/mbedtls_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/mbedtls_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
- [FRDM-MCXW72](../../_boards/frdmmcxw72/mbedtls_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/mbedtls_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
- [MCX-W72-LOC](../../_boards/mcxw72loc/mbedtls_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/mbedtls_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
- [KW47-LOC](../../_boards/kw47loc/mbedtls_examples/psa_crypto_opaque_key_examples/example_board_readme.md)
