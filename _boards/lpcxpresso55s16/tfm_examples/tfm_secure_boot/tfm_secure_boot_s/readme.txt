Overview
========
The Trusted Firmware M (TF-M) certification application which is provided to a PSA Certification laboratory.

NOTE: The TF-M main() functions have a non-standard location:
       - Secure main() is in the tf-m\secure_fw\spm\cmsis_psa\main.c file.
       - Non-Secure main() is in the tf-m-tests\app\main_ns.c file.

Toolchain supported
===================
- Keil MDK  5.34

Hardware requirements
=====================
- Mini/micro USB cable
- LPCXpresso55S16 board
- Personal Computer

Board settings
==============
No special settings are required.

Prepare the Demo
================
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port (J1) on the board
2.  Open a serial terminal with the following settings (See Appendix A in Getting started guide for description how to determine serial port number):
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Use secure project to download the program to target board
    For IAR environment go to project options/Debugger/Images. Open file dialog for first extra image. Select file <application_name>_ns.out and click on open button. 
    Now you have absolute path in the path box. The flash loader requires absolute path to this file.
4.  Either press the reset button on your board or launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the TFM PSA API tests in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

[Sec Thread] Secure image initializing!
Booting TFM v1.1
Non-Secure system starting...

***** PSA Architecture Test Suite - Version 1.0 *****

Running.. Crypto Suite
******************************************

TEST: 201 | DESCRIPTION: Testing psa_crypto_init API: Basic
[Info] Executing tests from non-secure
[Check 1] Test calling crypto functions before psa_crypto_init
TEST RESULT: SKIPPED (Skip Code=0x29)

******************************************

TEST: 202 | DESCRIPTION: Testing crypto key management APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_import_key 16 Byte AES
[Check 2] Test psa_import_key 24 Byte AES
[Check 3] Test psa_import_key 32 Byte AES
[Check 4] Test psa_import_key with EC Public key
[Check 5] Test psa_import_key with key data greater than the algorithm size
[Check 6] Test psa_import_key with incorrect key data size
[Check 7] Test psa_import_key with incorrect key type
TEST RESULT: PASSED

******************************************

TEST: 203 | DESCRIPTION: Testing crypto key management APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_export_key 16 Byte AES
[Check 2] Test psa_export_key 24 Byte AES
[Check 3] Test psa_export_key 32 Byte AES
[Check 4] Test psa_export_key with EC Public key
[Check 5] Test psa_export_key with key policy verify
[Check 6] Test psa_export_key with less buffer size
TEST RESULT: PASSED

******************************************

TEST: 204 | DESCRIPTION: Testing crypto key management APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_export_public_key 16 Byte AES
[Check 2] Test psa_export_public_key 24 Byte AES
[Check 3] Test psa_export_public_key 32 Byte AES
[Check 4] Test psa_export_public_key with EC Public key
TEST RESULT: PASSED

******************************************

TEST: 205 | DESCRIPTION: Testing crypto key management APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_destroy_key 16 Byte AES
[Check 2] Test psa_destroy_key 24 Byte AES
[Check 3] Test psa_destroy_key 32 Byte AES
[Check 4] Test psa_destroy_key with EC Public key
TEST RESULT: PASSED

******************************************

TEST: 208 | DESCRIPTION: Testing crypto key derivation APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_setup - ECDH + HKDF-SHA-256
[Check 2] Test psa_key_derivation_setup - ECDH, unknown KDF
[Check 3] Test psa_key_derivation_setup - bad key derivation algorithm
[Check 4] Test psa_key_derivation_setup - Invalid Algorithm
TEST RESULT: PASSED

******************************************

TEST: 210 | DESCRIPTION: Testing crypto key attributes APIs
[Info] Executing tests from non-secure
[Check 1] Test set/get key attributes
TEST RESULT: PASSED

******************************************

TEST: 211 | DESCRIPTION: Testing crypto hash functions APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_setup with SHA256 algorithm
[Check 2] Test psa_hash_setup with Invalid hash algorithm
[Check 3] Test psa_hash_setup with Invalid algorithm
[Check 4] Test psa_hash_setup with CTR algorithm
TEST RESULT: PASSED

******************************************

TEST: 212 | DESCRIPTION: Testing crypto hash functions APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_update with SHA256 algorithm
[Check 2] Test psa_hash_update without hash setup
[Check 3] Test psa_hash_update with completed opertaion handle
TEST RESULT: PASSED

******************************************

TEST: 213 | DESCRIPTION: Testing crypto hash functions APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_verify with SHA256 algorithm
[Check 2] Test psa_hash_verify with incorrect expected hash
[Check 3] Test psa_hash_verify with incorrect hash length
[Check 4] test psa_hash_verify with inactive & invalid operation handle
TEST RESULT: PASSED

******************************************

TEST: 214 | DESCRIPTION: Testing crypto hash functions APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_finish with SHA256 algorithm
[Check 2] test psa_hash_finish with inactive operation handle
[Check 3] test psa_hash_finish with invalid hash buffer size
TEST RESULT: PASSED

******************************************

TEST: 215 | DESCRIPTION: Testing crypto hash functions APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_abort with SHA256 algorithm
[Check 2] Test psa_hash_finish after calling psa_hash_abort
TEST RESULT: PASSED

******************************************

TEST: 216 | DESCRIPTION: Testing crypto generator functions APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_generate_key 16 Byte AES
[Check 2] Test psa_generate_key 24 Byte AES
[Check 3] Test psa_generate_key 32 Byte AES
[Check 4] Test psa_generate_key with invalid key type
[Check 5] Test psa_generate_key with invalid usage flags
TEST RESULT: PASSED

******************************************

TEST: 217 | DESCRIPTION: Testing crypto generation APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_generate_random to get 0 Byte data
[Check 2] Test psa_generate_random to get 16 Byte data
[Check 3] Test psa_generate_random to get 24 Byte data
[Check 4] Test psa_generate_random to get 32 Byte data
[Check 5] Test psa_generate_random to get 64 Byte data
[Check 6] Test psa_generate_random to get 128 Byte data
[Check 7] Test psa_generate_random to get 256 Byte data
[Check 8] Test psa_generate_random to get 512 Byte data
[Check 9] Test psa_generate_random to get 1000 Byte data
TEST RESULT: PASSED

******************************************

TEST: 218 | DESCRIPTION: Testing crypto key derivation APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_input_key 16 Byte Key
[Check 2] Test psa_key_derivation_input_key with invalid usage
[Check 3] Test psa_key_derivation_input_key with step as label
[Check 4] Test psa_key_derivation_input_key with step as info
[Check 5] Test psa_key_derivation_input_key with step as seed
[Check 6] Test psa_key_derivation_input_key with step as salt
[Check 7] Test psa_key_derivation_input_key with key type as AES(not derive)
[Check 8] Test psa_key_derivation_input_key incorrect key algorithm
[Check 9] Test psa_key_derivation_input_key with zero as step
[Check 10] Test psa_key_derivation_input_key invalid handle
TEST RESULT: PASSED

******************************************

TEST: 219 | DESCRIPTION: Testing crypto key derivation APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_key_agreement - KDF instead of a key agreement algorithm
TEST RESULT: PASSED

******************************************

TEST: 222 | DESCRIPTION: Testing crypto key derivation APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_abort
TEST RESULT: PASSED

******************************************

TEST: 223 | DESCRIPTION: Testing crypto key derivation APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_set_get_capacity - Less than operation's capacity
[Check 2] Test psa_key_derivation_set_get_capacity - Equal to operation's capacity
[Check 3] Test psa_key_derivation_set_get_capacity - More than operation's capacity
TEST RESULT: PASSED

******************************************

TEST: 224 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_encrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 2] Test psa_aead_encrypt - AES-CCM
[Check 3] Test psa_aead_encrypt - AES-CCM 24 bytes Tag length = 4
[Check 4] Test psa_aead_encrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 5] Test psa_aead_encrypt - Unsupported Algorithm
[Check 6] Test psa_aead_encrypt - Invalid key usage
[Check 7] Test psa_aead_encrypt - Small output buffer size
TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_decrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 2] Test psa_aead_decrypt - AES-CCM
[Check 3] Test psa_aead_decrypt - AES-CCM 24 bytes Tag length = 4
[Check 4] Test psa_aead_decrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 5] Test psa_aead_decrypt - Unsupported Algorithm
[Check 6] Test psa_aead_decrypt - Invalid key usage
[Check 7] Test psa_aead_decrypt - Small output buffer size
[Check 8] Test psa_aead_decrypt - Invalid cipher text
[Check 9] Test psa_aead_decrypt - Invalid cipher text size
[Check 10] Test psa_aead_decrypt - Invalid tag length 0
TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 11] Test psa_aead_decrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 12] Test psa_aead_decrypt - AES-CCM
[Check 13] Test psa_aead_decrypt - AES-CCM 24 bytes Tag length = 4
[Check 14] Test psa_aead_decrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 15] Test psa_aead_decrypt - Unsupported Algorithm
[Check 16] Test psa_aead_decrypt - Invalid key usage
[Check 17] Test psa_aead_decrypt - Small output buffer size
[Check 18] Test psa_aead_decrypt - Invalid cipher text
[Check 19] Test psa_aead_decrypt - Invalid cipher text size
[Check 20] Test psa_aead_decrypt - Invalid tag length 0
TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 21] Test psa_aead_decrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 22] Test psa_aead_decrypt - AES-CCM
[Check 23] Test psa_aead_decrypt - AES-CCM 24 bytes Tag length = 4
[Check 24] Test psa_aead_decrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 25] Test psa_aead_decrypt - Unsupported Algorithm
[Check 26] Test psa_aead_decrypt - Invalid key usage
[Check 27] Test psa_aead_decrypt - Small output buffer size
[Check 28] Test psa_aead_decrypt - Invalid cipher text
[Check 29] Test psa_aead_decrypt - Invalid cipher text size
[Check 30] Test psa_aead_decrypt - Invalid tag length 0
TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 31] Test psa_aead_decrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 32] Test psa_aead_decrypt - AES-CCM
[Check 33] Test psa_aead_decrypt - AES-CCM 24 bytes Tag length = 4
[Check 34] Test psa_aead_decrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 35] Test psa_aead_decrypt - Unsupported Algorithm
[Check 36] Test psa_aead_decrypt - Invalid key usage
[Check 37] Test psa_aead_decrypt - Small output buffer size
[Check 38] Test psa_aead_decrypt - Invalid cipher text
[Check 39] Test psa_aead_decrypt - Invalid cipher text size
[Check 40] Test psa_aead_decrypt - Invalid tag length 0
TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 41] Test psa_aead_decrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 42] Test psa_aead_decrypt - AES-CCM
[Check 43] Test psa_aead_decrypt - AES-CCM 24 bytes Tag length = 4
[Check 44] Test psa_aead_decrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 45] Test psa_aead_decrypt - Unsupported Algorithm
[Check 46] Test psa_aead_decrypt - Invalid key usage
[Check 47] Test psa_aead_decrypt - Small output buffer size
[Check 48] Test psa_aead_decrypt - Invalid cipher text
[Check 49] Test psa_aead_decrypt - Invalid cipher text size
[Check 50] Test psa_aead_decrypt - Invalid tag length 0
TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 51] Test psa_aead_decrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 52] Test psa_aead_decrypt - AES-CCM
[Check 53] Test psa_aead_decrypt - AES-CCM 24 bytes Tag length = 4
[Check 54] Test psa_aead_decrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 55] Test psa_aead_decrypt - Unsupported Algorithm
[Check 56] Test psa_aead_decrypt - Invalid key usage
[Check 57] Test psa_aead_decrypt - Small output buffer size
[Check 58] Test psa_aead_decrypt - Invalid cipher text
[Check 59] Test psa_aead_decrypt - Invalid cipher text size
[Check 60] Test psa_aead_decrypt - Invalid tag length 0
TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs
[Info] Executing tests from non-secure
[Check 61] Test psa_aead_decrypt - CCM - 16B AES - 13B nounce & 8B addi data
[Check 62] Test psa_aead_decrypt - AES-CCM
[Check 63] Test psa_aead_decrypt - AES-CCM 24 bytes Tag length = 4
[Check 64] Test psa_aead_decrypt - GCM - 16B AES - 12B Nounce & 12B addi data
[Check 65] Test psa_aead_decrypt - Unsupported Algorithm
[Check 66] Test psa_aead_decrypt - Invalid key usage
[Check 67] Test psa_aead_decrypt - Small output buffer size
[Check 68] Test psa_aead_decrypt - Invalid cipher text
[Check 69] Test psa_aead_decrypt - Invalid cipher text size
[Check 70] Test psa_aead_decrypt - Invalid tag length 0
TEST RESULT: PASSED

******************************************

TEST: 232 | DESCRIPTION: Testing crypto symmetric cipher APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_encrypt_setup 16 Byte AES
[Check 2] Test psa_cipher_encrypt_setup 24 Byte AES
[Check 3] Test psa_cipher_encrypt_setup 32 Byte AES
[Check 4] Test psa_cipher_encrypt_setup 16 Byte raw data
[Check 5] Test psa_cipher_encrypt_setup - not a cipher algorithm
[Check 6] Test psa_cipher_encrypt_setup - unknown cipher algorithm
[Check 7] Test psa_cipher_encrypt_setup - incorrect usage
[Check 8] Test psa_cipher_encrypt_setup - EC Public key
[Check 9] Test psa_cipher_encrypt_setup - Invalid key handle
[Check 10] Test psa_cipher_encrypt_setup - Zero as key handle
TEST RESULT: PASSED

******************************************

TEST: 233 | DESCRIPTION: Testing crypto symmetric cipher APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_decrypt_setup 16 Byte AES
[Check 2] Test psa_cipher_decrypt_setup 24 Byte AES
[Check 3] Test psa_cipher_decrypt_setup 32 Byte AES
[Check 4] Test psa_cipher_decrypt_setup 16 Byte raw data
[Check 5] Test psa_cipher_decrypt_setup - not a cipher algorithm
[Check 6] Test psa_cipher_decrypt_setup - unknown cipher algorithm
[Check 7] Test psa_cipher_decrypt_setup - incorrect usage
[Check 8] Test psa_cipher_decrypt_setup - Invalid key handle
[Check 9] Test psa_cipher_decrypt_setup - Zero as key handle
TEST RESULT: PASSED

******************************************

TEST: 233 | DESCRIPTION: Testing crypto symmetric cipher APIs
[Info] Executing tests from non-secure
[Check 10] Test psa_cipher_decrypt_setup 16 Byte AES
[Check 11] Test psa_cipher_decrypt_setup 24 Byte AES
[Check 12] Test psa_cipher_decrypt_setup 32 Byte AES
[Check 13] Test psa_cipher_decrypt_setup 16 Byte raw data
[Check 14] Test psa_cipher_decrypt_setup - not a cipher algorithm
[Check 15] Test psa_cipher_decrypt_setup - unknown cipher algorithm
[Check 16] Test psa_cipher_decrypt_setup - incorrect usage
[Check 17] Test psa_cipher_decrypt_setup - Invalid key handle
[Check 18] Test psa_cipher_decrypt_setup - Zero as key handle
TEST RESULT: PASSED

******************************************

TEST: 235 | DESCRIPTION: Testing crypto symmetric cipher APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_set_iv 16 Byte AES
[Check 2] Test psa_cipher_set_iv 24 Byte AES
[Check 3] Test psa_cipher_set_iv 32 Byte AES
[Check 4] Test psa_cipher_set_iv AES - small iv buffer
[Check 5] Test psa_cipher_set_iv AES - large iv buffer
TEST RESULT: PASSED

******************************************

TEST: 236 | DESCRIPTION: Testing crypto symmetric cipher APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_update - Encrypt - AES CBC_NO_PADDING
[Check 2] Test psa_cipher_update - Encrypt - AES CBC_NO_PADDING (Short input)
[Check 3] Test psa_cipher_update - Encrypt - AES CTR
[Check 4] Test psa_cipher_update - small output buffer size
[Check 5] Test psa_cipher_update - Decrypt - AES CBC_NO_PADDING
[Check 6] Test psa_cipher_update - Decrypt - AES CBC_NO_PADDING (Short input)
[Check 7] Test psa_cipher_update - Decrypt - AES CTR
[Check 8] Test psa_cipher_update without cipher setup
TEST RESULT: PASSED

******************************************

TEST: 237 | DESCRIPTION: Testing crypto symmetric cipher APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_finish - Encrypt - AES CBC_NO_PADDING
[Check 2] Test psa_cipher_finish - Encrypt - AES CBC_NO_PADDING (Short input)
[Check 3] Test psa_cipher_finish - Encrypt - AES CTR
[Check 4] Test psa_cipher_finish - Encrypt - AES CTR (short input)
[Check 5] Test psa_cipher_finish - Decrypt - AES CBC_NO_PADDING
[Check 6] Test psa_cipher_finish - Decrypt - AES CBC_NO_PADDING (Short input)
[Check 7] Test psa_cipher_finish - Decrypt - AES CTR
[Check 8] Test psa_cipher_finish - Decrypt - AES CTR (short input)
TEST RESULT: PASSED

******************************************

TEST: 238 | DESCRIPTION: Testing crypto symmetric cipher APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_abort - Encrypt - AES CBC_NO_PADDING
[Check 2] Test psa_cipher_abort - Encrypt - AES CTR
[Check 3] Test psa_cipher_abort - Decrypt - AES CBC_NO_PADDING
[Check 4] Test psa_cipher_abort - Decrypt - AES CTR
[Check 5] Test psa_cipher_update after psa_cipher_abort should fail
TEST RESULT: PASSED

******************************************

TEST: 239 | DESCRIPTION: Testing crypto asymmetric APIs
[Info] Executing tests from non-secure
No test available for the selected crypto configuration
TEST RESULT: SKIPPED (Skip Code=0x2d)

******************************************

TEST: 240 | DESCRIPTION: Testing crypto asymmetric APIs
[Info] Executing tests from non-secure
No test available for the selected crypto configuration
TEST RESULT: SKIPPED (Skip Code=0x2d)

******************************************

TEST: 241 | DESCRIPTION: Testing crypto asymmetric APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_asymmetric_sign - ECDSA SECP256R1 SHA-256
TEST RESULT: PASSED

******************************************

TEST: 242 | DESCRIPTION: Testing crypto asymmetric APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_asymmetric_verify - ECDSA KEY_PAIR SECP256R1 SHA-256
[Check 2] Test psa_asymmetric_verify - EC public key
[Check 3] Test psa_asymmetric_verify - Wrong signature size
TEST RESULT: PASSED

******************************************

TEST: 243 | DESCRIPTION: Testing crypto key derivation APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_raw_key_agreement - Not a key agreement alg
TEST RESULT: PASSED

******************************************

TEST: 244 | DESCRIPTION: Testing crypto key management APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_copy_key 16 Byte AES
[Check 2] Test psa_copy_key without copy usage
[Check 3] Test psa_copy_key 24 Byte AES
[Check 4] Test psa_copy_key 32 Byte AES
[Check 5] Test psa_copy_key with EC Public key
[Check 6] Test psa_copy_key with Incompatible target policy
TEST RESULT: PASSED

******************************************

TEST: 245 | DESCRIPTION: Testing crypto hash functions APIs
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_clone with SHA256 algorithm
TEST RESULT: PASSED

******************************************

************ Crypto Suite Report **********
TOTAL TESTS     : 40
TOTAL PASSED    : 37
TOTAL SIM ERROR : 0
TOTAL FAILED    : 0
TOTAL SKIPPED   : 3
******************************************

Entering standby..

    
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

