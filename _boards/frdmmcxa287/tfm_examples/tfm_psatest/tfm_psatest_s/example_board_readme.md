Hardware requirements
=====================
- Micro USB cable
- FRDM-MCXL255 board
- Personal Computer

Board settings
============
No special settings are required.

Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the CMSIS DAP USB port on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Use secure project to download the program to target board. Please refer to "TrustZone application debugging" below for details.
4.  Launch the debugger in your IDE to begin running the demo.
Note: Refering to the "Getting started with MCUXpresso SDK for FRDM-MCXL255" documentation for more information
      on how to build and run the TrustZone examples in various IDEs.

Running the demo
================
The log below shows the output of the TFM PSA API tests in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Booting TF-M v2.1.1
[WRN] This device was provisioned with dummy keys. This device is NOT SECURE
[Sec Thread] Secure image initializing!
TF-M Float ABI: Hard
Lazy stacking enabled
Creating an empty ITS flash layout.
Creating an empty PS flash layout.
[INF][PS] Encryption alg: 0x5500100
Non-Secure system starting...

***** PSA Architecture Test Suite - Version 1.4 *****

Running.. Crypto Suite
******************************************

TEST: 201 | DESCRIPTION: Testing crypto key management APIs | UT:  psa_crypto_init
[Info] Executing tests from non-secure
[Check 1] Test calling crypto functions before psa_crypto_init
[Check 2] Test psa_crypto_init
[Check 3] Test multiple psa_crypto_init

TEST RESULT: PASSED

******************************************

TEST: 202 | DESCRIPTION: Testing crypto key management APIs | UT: psa_import_key
[Info] Executing tests from non-secure
[Check 1] Test psa_import_key with EC Public key

TEST RESULT: PASSED

******************************************

TEST: 203 | DESCRIPTION: Testing crypto key management APIs | UT: psa_export_key
[Info] Executing tests from non-secure
[Check 1] Test psa_export_key with EC Public key

TEST RESULT: PASSED

******************************************

TEST: 204 | DESCRIPTION: Testing crypto key management APIs | UT: psa_export_public_key
[Info] Executing tests from non-secure
[Check 1] Test psa_export_public_key with EC Public key

TEST RESULT: PASSED

******************************************

TEST: 205 | DESCRIPTION: Testing crypto key management APIs | UT: psa_destroy_key
[Info] Executing tests from non-secure
[Check 1] Test psa_destroy_key with EC Public key

TEST RESULT: PASSED

******************************************

TEST: 206 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_compute
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_compute with SHA256 algorithm
[Check 2] Test psa_hash_compute with invalid algorithm

TEST RESULT: PASSED

******************************************

TEST: 207 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_compare
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_compare with SHA256 algorithm
[Check 2] Test psa_hash_compare with incorrect hash
[Check 3] Test psa_hash_compare with incorrect hash length
[Check 4] Test psa_hash_compare with invalid algorithm

TEST RESULT: PASSED

******************************************

TEST: 208 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_key_derivation_setup
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_setup - ECDH + HKDF-SHA-256
[Check 2] Test psa_key_derivation_setup - ECDH, unknown KDF
[Check 3] Test psa_key_derivation_setup - bad key derivation algorithm
[Check 4] Test psa_key_derivation_setup - Invalid Algorithm

TEST RESULT: PASSED

******************************************

TEST: 209 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_key_derivation_input_bytes
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_input_bytes - Step as Info
[Check 2] Test psa_key_derivation_input_bytes - Step as secret
[Check 3] Test psa_key_derivation_input_bytes - Step as salt
[Check 4] Test psa_key_derivation_input_bytes - Step as label
[Check 5] Test psa_key_derivation_input_bytes - Step as seed
[Check 6] Test psa_key_derivation_input_bytes - Invalid step

TEST RESULT: PASSED

******************************************

TEST: 210 | DESCRIPTION: Testing crypto key attributes APIs | UT: psa_key_attributes_set_get
[Info] Executing tests from non-secure
[Check 1] Test psa_key_attributes_set_get key attributes

TEST RESULT: PASSED

******************************************

TEST: 211 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_setup
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_setup with SHA256 algorithm
[Check 2] Test psa_hash_setup with Invalid hash algorithm
[Check 3] Test psa_hash_setup with Invalid algorithm
[Check 4] Test psa_hash_setup with CTR algorithm

TEST RESULT: PASSED

******************************************

TEST: 212 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_update
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_update with SHA256 algorithm
[Check 2] Test psa_hash_update without hash setup
[Check 3] Test psa_hash_update with completed opertaion handle

TEST RESULT: PASSED

******************************************

TEST: 213 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_verify
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_verify with SHA256 algorithm
[Check 2] Test psa_hash_verify with incorrect expected hash
[Check 3] Test psa_hash_verify with incorrect hash length
[Check 4] test psa_hash_verify with inactive & invalid operation handle

TEST RESULT: PASSED

******************************************

TEST: 214 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_finish
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_finish with SHA256 algorithm
[Check 2] Test psa_hash_finish with invalid hash buffer size
[Check 3] test psa_hash_finish with inactive operation handle

TEST RESULT: PASSED

******************************************

TEST: 215 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_abort
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_abort with SHA256 algorithm
[Check 2] Test psa_hash_finish after calling psa_hash_abort

TEST RESULT: PASSED

******************************************

TEST: 216 | DESCRIPTION: Testing crypto generator functions APIs | UT: psa_generate_key
[Info] Executing tests from non-secure
[Check 1] Test psa_generate_key with invalid key type
[Check 2] Test psa_generate_key with invalid usage flags

TEST RESULT: PASSED

******************************************

TEST: 217 | DESCRIPTION: Testing crypto generation APIs | UT: psa_generate_random
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

TEST: 218 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_key_derivation_input_key
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
[Check 10] Test psa_cipher_decrypt_setup - Invalid key handle
[Check 11] Test psa_cipher_decrypt_setup - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 219 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_key_derivation_key_agreement
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_key_agreement - KDF not a key agreement alg

TEST RESULT: SKIPPED (Skip Code=0x2d)

******************************************

TEST: 220 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_key_derivation_output_bytes
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_output_bytes - HKDF
[Check 2] Test psa_key_derivation_output_bytes - optional salt
[Check 3] Test psa_key_derivation_output_bytes - capacity < output_length
[Check 4] Test psa_key_derivation_output_bytes - missing info
[Check 5] Test psa_key_derivation_output_bytes - missing salt/secret/info
[Check 6] Test psa_key_derivation_output_bytes - TLS12_PRF
[Check 7] Test psa_key_derivation_output_bytes - capacity < output_length
[Check 8] Test psa_key_derivation_output_bytes - missing seed/secret/label

TEST RESULT: PASSED

******************************************

TEST: 221 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_key_derivation_output_key
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_output_key - Key
[Check 2] Test psa_key_derivation_output_key - Info
[Check 3] Test psa_key_derivation_output_key - Salt
[Check 4] Test psa_key_derivation_output_key - Greater Capacity than available
[Check 5] Test psa_key_derivation_output_key - ECC Public key
[Check 6] Test psa_key_derivation_output_key -  ECC keypair (wrong key length)
[Check 7] Test psa_key_derivation_output_key -  RSA Public Key[Check 8] Test psa_key_derivation_output_key -  RSA keypair
[Check 9] Test psa_key_derivation_output_key - Invalid key size

TEST RESULT: PASSED

******************************************

TEST: 222 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_key_derivation_abort
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_abort

TEST RESULT: PASSED

******************************************

TEST: 223 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_key_derivation_set_get_capacity
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_set_get_capacity - < operation's capacity
[Check 2] Test psa_key_derivation_set_get_capacity - = operation's capacity
[Check 3] Test psa_key_derivation_set_get_capacity - > operation's capacity
[Check 4] Test psa_key_derivation_set_get_capacity - unchanged capacity

TEST RESULT: PASSED

******************************************

TEST: 224 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_encrypt
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_encrypt - CCM - AES - 13B nonce & 8B add data
[Check 2] Test psa_aead_encrypt - CCM - AES - 13B nonce & 32B add data
[Check 3] Test psa_aead_encrypt - CCM - AES - 24 bytes Tag length = 4
[Check 4] Test psa_aead_encrypt - CCM - AES - Zero additional data
[Check 5] Test psa_aead_encrypt - CCM - AES - Zero plaintext
[Check 6] Test psa_aead_encrypt - Unsupported algorithm
[Check 7] Test psa_aead_encrypt - Invalid usage flag
[Check 8] Test psa_aead_encrypt - Invalid ciphertext_size
[Check 9] Test psa_aead_encrypt - Invalid nonce
[Check 10] Test psa_aead_encrypt - Invalid tag length 0

TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_decrypt
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_decrypt - CCM - AES - 13B nonce & 8B add data
[Check 2] Test psa_aead_decrypt - CCM - AES - 13B nonce & 32B add data
[Check 3] Test psa_aead_decrypt - CCM - AES - 24 bytes Tag length = 4
[Check 4] Test psa_aead_decrypt - CCM - AES - Zero additional data
[Check 5] Test psa_aead_decrypt - CCM - AES - Zero plaintext
[Check 6] Test psa_aead_decrypt - Unsupported algorithm
[Check 7] Test psa_aead_decrypt - Invalid usage flag
[Check 8] Test psa_aead_decrypt - Invalid plaintext_size
[Check 9] Test psa_aead_decrypt - Invalid nonce
[Check 10] Test psa_aead_decrypt - Invalid cihpertext
[Check 11] Test psa_aead_decrypt - Invalid cihpertext_size
[Check 12] Test psa_aead_decrypt - Invalid tag length 0

TEST RESULT: PASSED

******************************************

TEST: 226 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_sign_setup
[Info] Executing tests from non-secure
[Check 1] Test psa_mac_sign_setup - HMAC - SHA256
[Check 2] Test psa_mac_sign_setup - Invalid usage flag
[Check 3] Test psa_mac_sign_setup - Invalid key type
[Check 4] Test psa_mac_sign_setup - Truncated MAC too large
[Check 5] Test psa_mac_sign_setup - Truncated MAC too small
[Check 6] Test psa_mac_sign_setup - Unknown MAC algorithm
[Check 7] Test psa_mac_sign_setup invalid key handle
[Check 8] Test psa_mac_sign_setup zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 227 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_update
[Info] Executing tests from non-secure
[Check 1] Test psa_mac_update - HMAC - SHA256 - 64 Byte

TEST RESULT: PASSED

******************************************

TEST: 228 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_sign_finish
[Info] Executing tests from non-secure
[Check 1] Test psa_mac_sign_finish  - HMAC - SHA256

TEST RESULT: PASSED

******************************************

TEST: 229 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_verify_setup
[Info] Executing tests from non-secure
[Check 1] Test psa_mac_verify_setup - HMAC - SHA256
[Check 2] Test psa_mac_verify_setup - Invalid usage flag
[Check 3] Test psa_mac_verify_setup - Invalid key type
[Check 4] Test psa_mac_verify_setup - Truncated MAC too large
[Check 5] Test psa_mac_verify_setup - Truncated MAC too small
[Check 6] Test psa_mac_verify_setup - Unknown MAC algorithm
[Check 7] Test psa_mac_verify_setup invalid key handle
[Check 8] Test psa_mac_verify_setup zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 230 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_verify_finish
[Info] Executing tests from non-secure
[Check 1] Test psa_mac_verify_finish - HMAC - SHA256

TEST RESULT: PASSED

******************************************

TEST: 231 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_abort
[Info] Executing tests from non-secure
[Check 1] Test psa_mac_abort - HMAC - SHA256
[Check 2] Test psa_mac_sign_finish after calling psa_mac_abort

TEST RESULT: PASSED

******************************************

TEST: 232 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_encrypt_setup
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_encrypt_setup - unknown cipher algorithm
[Check 2] Test psa_cipher_encrypt_setup - EC Public key
[Check 3] Test psa_cipher_encrypt_setup - Invalid key handle
[Check 4] Test psa_cipher_encrypt_setup - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 233 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_decrypt_setup
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_decrypt_setup - unknown cipher algorithm
[Check 2] Test psa_cipher_decrypt_setup - EC Public key
[Check 3] Test psa_cipher_decrypt_setup - Invalid key handle
[Check 4] Test psa_cipher_decrypt_setup - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 234 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_generate_iv
[Info] Executing tests from non-secure
No test available for the selected crypto configuration

TEST RESULT: SKIPPED (Skip Code=0x2d)

******************************************

TEST: 235 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_set_iv
[Info] Executing tests from non-secure
No test available for the selected crypto configuration

TEST RESULT: SKIPPED (Skip Code=0x2d)

******************************************

TEST: 236 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_update
[Info] Executing tests from non-secure
No test available for the selected crypto configuration

TEST RESULT: SKIPPED (Skip Code=0x2d)

******************************************

TEST: 237 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_finish
[Info] Executing tests from non-secure
No test available for the selected crypto configuration

TEST RESULT: SKIPPED (Skip Code=0x2d)

******************************************

TEST: 238 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_abort
[Info] Executing tests from non-secure
No test available for the selected crypto configuration

TEST RESULT: SKIPPED (Skip Code=0x2d)

******************************************

TEST: 239 | DESCRIPTION: Testing crypto asymmetric APIs | UT: psa_asymmetric_encrypt
[Info] Executing tests from non-secure
No test available for the selected crypto configuration

TEST RESULT: SKIPPED (Skip Code=0x2d)

******************************************

TEST: 240 | DESCRIPTION: Testing crypto asymmetric APIs | UT: psa_asymmetric_decrypt
[Info] Executing tests from non-secure
No test available for the selected crypto configuration

TEST RESULT: SKIPPED (Skip Code=0x2d)

******************************************

TEST: 241 | DESCRIPTION: Testing crypto asymmetric APIs | UT: psa_sign_hash
[Info] Executing tests from non-secure
[Check 1] Test psa_sign_hash - ECDSA SECP256R1 SHA-256
[Check 2] Test psa_sign_hash - Invalid key handle
[Check 3] Test psa_sign_hash - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 242 | DESCRIPTION: Testing crypto asymmetric APIs | UT: psa_verify_hash
[Info] Executing tests from non-secure
[Check 1] Test psa_verify_hash - ECDSA KEY_PAIR SECP256R1 SHA-256
[Check 2] Test psa_verify_hash - EC public key
[Check 3] Test psa_verify_hash - Invalid key handle
[Check 4] Test psa_verify_hash - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 243 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_raw_key_agreement
[Info] Executing tests from non-secure
[Check 1] Test psa_raw_key_agreement - Not a key agreement alg
No test available for the selected crypto configuration

TEST RESULT: SKIPPED (Skip Code=0x2d)

******************************************

TEST: 244 | DESCRIPTION: Testing crypto key management APIs | UT: psa_copy_key
[Info] Executing tests from non-secure
[Check 1] Test psa_copy_key - EC Public key

TEST RESULT: PASSED

******************************************

TEST: 245 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_clone
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_clone - SHA256 algorithm
[Check 2] Test psa_hash_clone - from an inactive source operation
[Check 3] Test psa_hash_clone - on an active target operation

TEST RESULT: PASSED

******************************************

TEST: 246 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_compute
[Info] Executing tests from non-secure
[Check 1] Test psa_mac_compute HMAC SHA 256

TEST RESULT: PASSED

******************************************

TEST: 247 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_verify
[Info] Executing tests from non-secure
[Check 1] Test psa_mac_verify HMAC SHA 256
[Check 2] Test psa_mac_verify - Incompactible HMAC for CMAC
[Check 3] Test psa_mac_verify - Invalid usage
[Check 4] Test psa_mac_verify - Truncated MAC too large
[Check 5] Test psa_mac_verify - Truncated MAC too small
[Check 6] Test psa_mac_verify - bad algorithm (unknown MAC algorithm)
[Check 7] Test psa_mac_verify - Invalid key type
[Check 8] Test psa_mac_verify small size buffer

TEST RESULT: PASSED

******************************************

TEST: 248 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_encrypt
[Info] Executing tests from non-secure
No test available for the selected crypto configuration

TEST RESULT: SKIPPED (Skip Code=0x2d)

******************************************

TEST: 249 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_decrypt
[Info] Executing tests from non-secure
No test available for the selected crypto configuration

TEST RESULT: SKIPPED (Skip Code=0x2d)

******************************************

TEST: 250 | DESCRIPTION: Testing crypto key management APIs | UT: psa_sign_message
[Info] Executing tests from non-secure
[Check 1] Test psa_sign_message - ECDSA SECP256R1 SHA-256
[Check 2] Test psa_sign_message - Invalid key handle
[Check 3] Test psa_sign_message - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 251 | DESCRIPTION: Testing crypto key management APIs | UT: psa_verify_message
[Info] Executing tests from non-secure
[Check 1] Test psa_verify_message - ECDSA KEY_PAIR SECP256R1 SHA-256
[Check 2] Test psa_verify_message - ECDSA Public Key SECP256R1 SHA-256
[Check 3] Test psa_verify_message - Invalid key handle
[Check 4] Test psa_verify_message - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 252 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_encrypt_setup
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_encrypt_setup - CCM - AES
[Check 2] Test psa_aead_encrypt_setup - CCM - AES - Tag length = 4
[Check 3] Test psa_aead_encrypt_setup - CCM - AES - Mismatched tag length
[Check 4] Test psa_aead_encrypt_setup - CCM - AES - Default Tag length

TEST RESULT: PASSED

******************************************

TEST: 253 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_decrypt_setup
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_decrypt_setup - CCM - AES
[Check 2] Test psa_aead_decrypt_setup - CCM - AES - Tag length = 4
[Check 3] Test psa_aead_decrypt_setup - CCM - AES - Mismatched tag length
[Check 4] Test psa_aead_decrypt_setup - CCM - AES - Default Tag length

TEST RESULT: PASSED

******************************************

TEST: 254 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_generate_nonce
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_generate_nonce - CCM
[Check 2] Test psa_aead_generate_nonce - CCM - Tag length = 4
[Check 3] Test psa_aead_generate_nonce - CCM - Small buffer size
[Check 4] Test psa_aead_generate_nonce - CCM - Uninitialized operation
[Check 5] Test psa_aead_generate_nonce - CCM - Decrypt operation

TEST RESULT: PASSED

******************************************

TEST: 255 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_set_nonce
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_set_nonce - Encrypt - CCM
[Check 2] Test psa_aead_set_nonce - Encrypt - CCM - Tag length = 4
[Check 3] Test psa_aead_set_nonce - Encrypt - CCM - Small nonce size
[Check 4] Test psa_aead_set_nonce - Encrypt - CCM - Large nonce size
[Check 5] Test psa_aead_set_nonce - Encrypt - CCM - Invalid operation state
[Check 6] Test psa_aead_set_nonce - Decrypt - CCM
[Check 7] Test psa_aead_set_nonce - Decrypt - CCM - Tag length = 4
[Check 8] Test psa_aead_set_nonce - Decrypt - CCM - Small nonce size
[Check 9] Test psa_aead_set_nonce - Decrypt - CCM - Large nonce size
[Check 10] Test psa_aead_set_nonce - Decrypt - CCM - Invalid operation state

TEST RESULT: PASSED

******************************************

TEST: 256 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_set_lengths
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_set_lengths - Encrypt - CCM
[Check 2] Test psa_aead_set_lengths - Encrypt - CCM - Tag length = 4
[Check 3] Test psa_aead_set_lengths - Encrypt - CCM - Zero ad_length
[Check 4] Test psa_aead_set_lengths - Encrypt - CCM - Zero plaintext_length
[Check 5] Test psa_aead_set_lengths - Encrypt - CCM - Invalid operation state
[Check 6] Test psa_aead_set_lengths - Decrypt - CCM
[Check 7] Test psa_aead_set_lengths - Decrypt - CCM - Tag length = 4
[Check 8] Test psa_aead_set_lengths - Decrypt - CCM - Zero ad_length
[Check 9] Test psa_aead_set_lengths - Decrypt - CCM - Zero plaintext_length
[Check 10] Test psa_aead_set_lengths - Decrypt - CCM - Invalid operation state

TEST RESULT: PASSED

******************************************

TEST: 257 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_update_ad
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_update_ad - Encrypt - CCM
[Check 2] Test psa_aead_update_ad - Encrypt - CCM - Tag length = 4
[Check 3] Test psa_aead_update_ad - Encrypt - CCM - Zero ad_length
[Check 4] Test psa_aead_update_ad - Encrypt - CCM - Zero plaintext_length
[Check 5] Test psa_aead_update_ad - Encrypt - CCM - Invalid operation state
[Check 6] Test psa_aead_update_ad - Encrypt - CCM - Overflow input length
[Check 7] Test psa_aead_update_ad - Decrypt - CCM
[Check 8] Test psa_aead_update_ad - Decrypt - CCM - Tag length = 4
[Check 9] Test psa_aead_update_ad - Decrypt - CCM - Zero ad_length
[Check 10] Test psa_aead_update_ad - Decrypt - CCM - Zero plaintext_length
[Check 11] Test psa_aead_update_ad - Decrypt - CCM - Invalid operation state
[Check 12] Test psa_aead_update_ad - Decrypt - CCM - Overflow input length

TEST RESULT: PASSED

******************************************

TEST: 258 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_update
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_update - Encrypt - CCM
[Check 2] Test psa_aead_update - Encrypt - CCM - Tag length = 4
[Check 3] Test psa_aead_update - Encrypt - CCM - Zero ad_length
[Check 4] Test psa_aead_update - Encrypt - CCM - Zero plaintext_length
[Check 5] Test psa_aead_update - Encrypt - CCM - Small buffer size
[Check 6] Test psa_aead_update - Encrypt - CCM - Less add data than specified
[Check 7] Test psa_aead_update - Encrypt - CCM - Overflow input length
[Check 8] Test psa_aead_update - Encrypt - CCM - Invalid operation state
[Check 9] Test psa_aead_update - Decrypt - CCM
[Check 10] Test psa_aead_update - Decrypt - CCM - Tag length = 4
[Check 11] Test psa_aead_update - Decrypt - CCM - Zero ad_length
[Check 12] Test psa_aead_update - Decrypt - CCM - Zero plaintext_length
[Check 13] Test psa_aead_update - Decrypt - CCM - Small buffer size
[Check 14] Test psa_aead_update - Decrypt - Less add data than specified
[Check 15] Test psa_aead_update - Decrypt - CCM - Overflow input length
[Check 16] Test psa_aead_update - Decrypt - CCM - Invalid operation state

TEST RESULT: PASSED

******************************************

TEST: 259 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_finish
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_finish - AES-CCM
[Check 2] Test psa_aead_finish - AES-CCM 24 bytes Tag length = 4
[Check 3] Test psa_aead_finish - Small buffer size
[Check 4] Test psa_aead_finish - Input length is less than plaintext length

TEST RESULT: PASSED

******************************************

TEST: 260 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_abort
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_abort - Encrypt - CCM - AES
[Check 2] Test psa_aead_abort - Decrypt - CCM - AES
[Check 3] Test psa_aead_abort with all initializations

TEST RESULT: PASSED

******************************************

TEST: 261 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_verify
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_verify - AES-CCM
[Check 2] Test psa_aead_verify - AES-CCM 24 bytes Tag length = 4
[Check 3] Test psa_aead_verify - Small buffer size
[Check 4] Test psa_aead_verify - Input length is less than plaintext length

TEST RESULT: PASSED

******************************************

************ Crypto Suite Report **********
TOTAL TESTS     : 61
TOTAL PASSED    : 50
TOTAL SIM ERROR : 0
TOTAL FAILED    : 0
TOTAL SKIPPED   : 11
******************************************

Running.. Attestation Suite
******************************************

TEST: 601 | DESCRIPTION: Testing attestation initial attestation APIs | UT: psa_initial_attestation
[Info] Executing tests from non-secure
[Check 1] Test psa_initial_attestation_get_token with Challenge 32
[Check 2] Test psa_initial_attestation_get_token with Challenge 48
[Check 3] Test psa_initial_attestation_get_token with Challenge 64
[Check 4] Test psa_initial_attestation_get_token with zero challenge size
[Check 5] Test psa_initial_attestation_get_token with small challenge size
[Check 6] Test psa_initial_attestation_get_token with invalid challenge size
[Check 7] Test psa_initial_attestation_get_token with large challenge size
[Check 8] Test psa_initial_attestation_get_token with zero as token size
[Check 9] Test psa_initial_attestation_get_token with small token size
[Check 10] Test psa_initial_attestation_get_token_size with Challenge 32
[Check 11] Test psa_initial_attestation_get_token_size with Challenge 48
[Check 12] Test psa_initial_attestation_get_token_size with Challenge 64
[Check 13] Test psa_initial_attestation_get_token_size with zero challenge size
[Check 14] Test psa_initial_attestation_get_token_size with small challenge size
[Check 15] Test psa_initial_attestation_get_token_size with invalid challenge size
[Check 16] Test psa_initial_attestation_get_token_size with large challenge size

TEST RESULT: PASSED

******************************************

************ Attestation Suite Report **********
TOTAL TESTS     : 1
TOTAL PASSED    : 1
TOTAL SIM ERROR : 0
TOTAL FAILED    : 0
TOTAL SKIPPED   : 0
******************************************

Running.. Storage Suite
******************************************

TEST: 401 | DESCRIPTION: UID not found check | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing ITS tests
[Check 1] Call get API for UID 6 which is not set
[Check 2] Call get_info API for UID 6 which is not set
[Check 3] Call remove API for UID 6 which is not set
[Check 4] Call get API for UID 6 which is removed
[Check 5] Call get_info API for UID 6 which is removed
[Check 6] Call remove API for UID 6 which is removed
Set storage for UID 6
[Check 7] Call get API for different UID 5
[Check 8] Call get_info API for different UID 5
[Check 9] Call remove API for different UID 5

[Info] Executing PS tests
[Check 1] Call get API for UID 6 which is not set
[Check 2] Call get_info API for UID 6 which is not set
[Check 3] Call remove API for UID 6 which is not set
[Check 4] Call get API for UID 6 which is removed
[Check 5] Call get_info API for UID 6 which is removed
[Check 6] Call remove API for UID 6 which is removed
Set storage for UID 6
[Check 7] Call get API for different UID 5
[Check 8] Call get_info API for different UID 5
[Check 9] Call remove API for different UID 5

TEST RESULT: PASSED

******************************************

TEST: 402 | DESCRIPTION: Write once error check | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing ITS tests
[Check 1] Update the flag of UID 1 with WRITE_ONCE flag
[Check 2] Try to remove the UID 1 having WRITE_ONCE flag
[Check 3] Create a new UID 2 with WRITE_ONCE flag
[Check 4] Try to remove the UID 2 having WRITE_ONCE flag
[Check 5] Try to change the length of write_once UID 2
[Check 6] Check UID removal still fails
[Check 7] Try to change the WRITE_ONCE flag to None for UID 2
[Check 8] Check UID removal still fails

[Info] Executing PS tests
[Check 1] Update the flag of UID 1 with WRITE_ONCE flag
[Check 2] Try to remove the UID 1 having WRITE_ONCE flag
[Check 3] Create a new UID 2 with WRITE_ONCE flag
[Check 4] Try to remove the UID 2 having WRITE_ONCE flag
[Check 5] Try to change the length of write_once UID 2
[Check 6] Check UID removal still fails
[Check 7] Try to change the WRITE_ONCE flag to None for UID 2
[Check 8] Check UID removal still fails

TEST RESULT: PASSED

******************************************

TEST: 403 | DESCRIPTION: Insufficient space check | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing ITS tests
[Check 1] Overload storage space
UID 8 set failed due to insufficient space
Remove all registered UIDs
[Check 2] Overload storage again to verify all previous UID removed
UID 8 set failed due to insufficient space
Remove all registered UIDs

[Info] Executing PS tests
[Check 1] Overload storage space
UID 8 set failed due to insufficient space
Remove all registered UIDs
[Check 2] Overload storage again to verify all previous UID removed
UID 8 set failed due to insufficient space
Remove all registered UIDs

TEST RESULT: PASSED

******************************************

TEST: 404 | DESCRIPTION: Data Consistency check | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing ITS tests
[Check 1] Call set API with reduced length - TEST_BUFF_SIZE/2
[Check 2] Call get API with default length - TEST_BUFF_SIZE

[Info] Executing PS tests
[Check 1] Call set API with reduced length - TEST_BUFF_SIZE/2
[Check 2] Call get API with default length - TEST_BUFF_SIZE

TEST RESULT: PASSED

******************************************

TEST: 405 | DESCRIPTION: Success scenarios check | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing ITS tests
[Check 1] Set UID with data length zero and call storage APIs
[Check 2] Resetting the length check

[Info] Executing PS tests
[Check 1] Set UID with data length zero and call storage APIs
[Check 2] Resetting the length check

TEST RESULT: PASSED

******************************************

TEST: 406 | DESCRIPTION: Check for storage create flags | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing ITS tests
[Check 1] Call set API with flag - PSA_STORAGE_FLAG_NONE
[Check 2] Call set API with flag - PSA_STORAGE_FLAG_NO_CONFIDENTIALITY
[Check 3] Call set API with flag - PSA_STORAGE_FLAG_NO_REPLAY_PROTECTION

[Info] Executing PS tests
[Check 1] Call set API with flag - PSA_STORAGE_FLAG_NONE
[Check 2] Call set API with flag - PSA_STORAGE_FLAG_NO_CONFIDENTIALITY
[Check 3] Call set API with flag - PSA_STORAGE_FLAG_NO_REPLAY_PROTECTION

TEST RESULT: PASSED

******************************************

TEST: 407 | DESCRIPTION: Incorrect Size check | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing ITS tests
Create a valid Storage - TEST_BUFF_SIZE/2
Increase the length of storage - TEST_BUFF_SIZE
[Check 1] Call get API with old length - TEST_BUFF_SIZE/2
[Check 2] Call get API with old length - TEST_BUFF_SIZE/4
Decrease the length of storage - TEST_BUFF_SIZE/4
[Check 3] Call get API with old length - TEST_BUFF_SIZE/2
[Check 4] Call get API with old length - TEST_BUFF_SIZE
[Check 5] Call get API with valid length - TEST_BUFF_SIZE/4

[Info] Executing PS tests
Create a valid Storage - TEST_BUFF_SIZE/2
Increase the length of storage - TEST_BUFF_SIZE
[Check 1] Call get API with old length - TEST_BUFF_SIZE/2
[Check 2] Call get API with old length - TEST_BUFF_SIZE/4
Decrease the length of storage - TEST_BUFF_SIZE/4
[Check 3] Call get API with old length - TEST_BUFF_SIZE/2
[Check 4] Call get API with old length - TEST_BUFF_SIZE
[Check 5] Call get API with valid length - TEST_BUFF_SIZE/4

TEST RESULT: PASSED

******************************************

TEST: 408 | DESCRIPTION: Invalid offset check | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing ITS tests
[Check 1] Try to access data with varying valid offset
[Check 2] Try to access data with varying invalid offset

[Info] Executing PS tests
[Check 1] Try to access data with varying valid offset
[Check 2] Try to access data with varying invalid offset

TEST RESULT: PASSED

******************************************

TEST: 409 | DESCRIPTION: Invalid Arguments check | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing ITS tests
[Check 1] Call set API with NULL pointer and data length 0
[Check 2] Call get API with NULL read buffer and data length 0
[Check 3] Remove the UID
[Check 4] Call get_info API to verify UID removed
[Check 5] Create UID with zero data_len and valid write buffer
[Check 8] Call get API with NULL read buffer and data length 0
[Check 9] Increase the length

[Info] Executing PS tests
[Check 1] Call set API with NULL pointer and data length 0
[Check 2] Call get API with NULL read buffer and data length 0
[Check 3] Remove the UID
[Check 4] Call get_info API to verify UID removed
[Check 5] Create UID with zero data_len and valid write buffer
[Check 8] Call get API with NULL read buffer and data length 0
[Check 9] Increase the length

TEST RESULT: PASSED

******************************************

TEST: 410 | DESCRIPTION: UID value zero check | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing ITS tests
[Check 1] Creating storage with UID 0 should fail
[Check 2] Get_info for UID 0 should fail
[Check 3] Removing storage with UID 0 should fail

[Info] Executing PS tests
[Check 1] Creating storage with UID 0 should fail
[Check 2] Get_info for UID 0 should fail
[Check 3] Removing storage with UID 0 should fail

TEST RESULT: PASSED

******************************************

TEST: 411 | DESCRIPTION: Optional APIs: UID not found check | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing PS tests
Test Case skipped as Optional PS APIs are not supported.

TEST RESULT: SKIPPED (Skip Code=0x2b)

******************************************

TEST: 412 | DESCRIPTION: Optional APIs: Invalid arguments and offset invalid | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing PS tests
Test Case skipped as Optional PS APIs are not supported.

TEST RESULT: SKIPPED (Skip Code=0x2b)

******************************************

TEST: 413 | DESCRIPTION: Set_Extended and Create api : Success | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing PS tests
Test Case skipped as Optional PS APIs are not supported.

TEST RESULT: SKIPPED (Skip Code=0x2b)

******************************************

TEST: 414 | DESCRIPTION: Optional APIs not supported check | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing PS tests
Optional PS APIs are not supported.
[Check 1] Call to create API should fail as API not supported
[Check 2] Create valid storage with set API
[Check 3] Call to set_extended API call should fail
[Check 4] Verify data is unchanged

TEST RESULT: PASSED

******************************************

TEST: 415 | DESCRIPTION: Create API write_once flag value check | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing PS tests
Test Case skipped as Optional PS APIs are not supported.

TEST RESULT: SKIPPED (Skip Code=0x2b)

******************************************

TEST: 416 | DESCRIPTION: Storage assest capacity modification check | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing PS tests
Test Case skipped as Optional PS APIs not are supported.

TEST RESULT: SKIPPED (Skip Code=0x2b)

******************************************

TEST: 417 | DESCRIPTION: Storage assest capacity modification check | UT: ITS
[Info] Executing tests from non-secure

[Info] Executing PS tests
Test Case skipped as Optional PS APIs not are supported.

TEST RESULT: SKIPPED (Skip Code=0x2b)

******************************************

************ Storage Suite Report **********
TOTAL TESTS     : 17
TOTAL PASSED    : 11
TOTAL SIM ERROR : 0
TOTAL FAILED    : 0
TOTAL SKIPPED   : 6
******************************************

Entering standby..

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~



TrustZone Application Development in SDK
----------------------------------------
Every TrustZone based application consists of two independent parts - secure part/project and non-secure part/project.

The secure project is stored in <application_name>\<application_name>_s directory.
The non-secure project is stored in <application_name>\<application_name>_ns directory. 

The secure projects always contains TrustZone configuration and it is executed after device RESET. The secure project usually
ends by jump to non-secure application/project.
If IDE allows the two projects in single workspace, the user can also find the project with <application_name>.
This project contains both secure and non-secure projects in one workspace (Keil MDK, IAR) and it allows to user easy transition from
one to another project.

TrustZone application compilation
--------------------------------
Please compile secure project firstly since CMSE library is needed for compilation of non-secure project.
After successful compilation of secure project, compile non-secure project.

TrustZone application debugging
-------------------------------
- Download both output file into device memory
- Start execution of secure project since secure project is going to be executed after device RESET.

If IDE (Keil MDK, IAR) allows to manage download both output files as single download, the secure project
is configured to download both secure and non-secure output files so debugging can be fully managed
from secure project.

Device header file and secure/non-secure access to the peripherals
------------------------------------------------------------------
Both secure and non-secure project uses identical device header file. The access to secure and non-secure aliases for all peripherals
is managed using compiler macro __ARM_FEATURE_CMSE.

For secure project using <PERIPH_BASE> means access through secure alias (address bit A28=1), 
using <PERIPH_BASE>_NS means access through non-secure alias(address bit A28=0)
For non-secure project using <PERIPH_BASE> means access through non-secure alias (address bit A28=0). 
The non-secure project doesn't have access to secure memory or peripherals regions so the secure access is not defined.

