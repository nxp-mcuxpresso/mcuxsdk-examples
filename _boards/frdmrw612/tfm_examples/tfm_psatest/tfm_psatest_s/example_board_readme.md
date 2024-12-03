Hardware requirements
=====================
- Micro USB cable
- FRDM-RW612 board
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
4.  Launch the debugger to begin running the demo.

Running the demo
================
The log below shows the output of the TFM PSA API tests in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
[INF] Beginning TF-M provisioning
[WRN] TFM_DUMMY_PROVISIONING is not suitable for production! This device is NOT SECURE
[Sec Thread] Secure image initializing!
TF-M Float ABI: Hard
Lazy stacking enabled
Booting TF-M 1.7.0
Creating an empty ITS flash layout.
Creating an empty PS flash layout.
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
[Check 1] Test psa_import_key 16 bytes AES
[Check 2] Test psa_import_key 24 bytes AES
[Check 3] Test psa_import_key 32 bytes AES
[Check 4] Test psa_import_key 2048 RSA public key
[Check 5] Test psa_import_key with RSA 2048 keypair
[Check 6] Test psa_import_key with EC Public key
[Check 7] Test psa_import_key with EC keypair
[Check 8] Test psa_import_key 16 bytes AES with invalid bits
[Check 9] Test psa_import_key with key data greater than the algorithm size
[Check 10] Test psa_import_key with incorrect key data size
[Check 11] Test psa_import_key with invalid key type value

TEST RESULT: PASSED

******************************************

TEST: 203 | DESCRIPTION: Testing crypto key management APIs | UT: psa_export_key
[Info] Executing tests from non-secure
[Check 1] Test psa_export_key 16 Byte AES
[Check 2] Test psa_export_key 24 Byte AES
[Check 3] Test psa_export_key 32 Byte AES
[Check 4] Test psa_export_key 2048 RSA public key
[Check 5] Test psa_export_key with RSA 2048 keypair
[Check 6] Test psa_export_key with EC Public key
[Check 7] Test psa_export_key with EC keypair
[Check 8] Test psa_export_key with key policy verify
[Check 9] Test psa_export_key with less buffer size

TEST RESULT: PASSED

******************************************

TEST: 204 | DESCRIPTION: Testing crypto key management APIs | UT: psa_export_public_key
[Info] Executing tests from non-secure
[Check 1] Test psa_export_public_key 16 Byte AES
[Check 2] Test psa_export_public_key 24 Byte AES
[Check 3] Test psa_export_public_key 32 Byte AES
[Check 4] Test psa_export_public_key 2048 RSA public key
[Check 5] Test psa_export_public_key with RSA 2048 keypair
[Check 6] Test psa_export_public_key with EC Public key
[Check 7] Test psa_export_public_key with EC keypair
[Check 8] Test psa_export_public_key with less buffer size

TEST RESULT: PASSED

******************************************

TEST: 205 | DESCRIPTION: Testing crypto key management APIs | UT: psa_destroy_key
[Info] Executing tests from non-secure
[Check 1] Test psa_destroy_key 16 Byte AES
[Check 2] Test psa_destroy_key 24 Byte AES
[Check 3] Test psa_destroy_key 32 Byte AES
[Check 4] Test psa_destroy_key 2048 RSA public key
[Check 5] Test psa_destroy_key with RSA 2048 keypair
[Check 6] Test psa_destroy_key with EC Public key
[Check 7] Test psa_destroy_key with EC keypair

TEST RESULT: PASSED

******************************************

TEST: 206 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_compute
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_compute with SHA256 algorithm
[Check 2] Test psa_hash_compute with SHA384 algorithm
[Check 3] Test psa_hash_compute with SHA512 algorithm
[Check 4] Test psa_hash_compute with small buffer size
[Check 5] Test psa_hash_compute with invalid algorithm

TEST RESULT: PASSED

******************************************

TEST: 207 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_compare
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_compare with SHA256 algorithm
[Check 2] Test psa_hash_compare with SHA384 algorithm
[Check 3] Test psa_hash_compare with SHA512 algorithm
[Check 4] Test psa_hash_compare with incorrect hash
[Check 5] Test psa_hash_compare with incorrect hash length
[Check 6] Test psa_hash_compare with invalid algorithm

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
[Check 2] Test psa_hash_setup with SHA384 algorithm
[Check 3] Test psa_hash_setup with SHA512 algorithm
[Check 4] Test psa_hash_setup with Invalid hash algorithm
[Check 5] Test psa_hash_setup with Invalid algorithm
[Check 6] Test psa_hash_setup with CTR algorithm

TEST RESULT: PASSED

******************************************

TEST: 212 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_update
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_update with SHA256 algorithm
[Check 2] Test psa_hash_update with SHA384 algorithm
[Check 3] Test psa_hash_update with SHA512 algorithm
[Check 4] Test psa_hash_update without hash setup
[Check 5] Test psa_hash_update with completed opertaion handle 

TEST RESULT: PASSED

******************************************

TEST: 213 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_verify
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_verify with SHA256 algorithm
[Check 2] Test psa_hash_verify with SHA384 algorithm
[Check 3] Test psa_hash_verify with SHA512 algorithm
[Check 4] Test psa_hash_verify with incorrect expected hash
[Check 5] Test psa_hash_verify with incorrect hash length
[Check 6] test psa_hash_verify with inactive & invalid operation handle

TEST RESULT: PASSED

******************************************

TEST: 214 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_finish
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_finish with SHA256 algorithm
[Check 2] Test psa_hash_finish with SHA384 algorithm
[Check 3] Test psa_hash_finish with SHA512 algorithm
[Check 4] Test psa_hash_finish with invalid hash buffer size
[Check 5] test psa_hash_finish with inactive operation handle

TEST RESULT: PASSED

******************************************

TEST: 215 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_abort
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_abort with SHA256 algorithm
[Check 2] Test psa_hash_abort with SHA384 algorithm
[Check 3] Test psa_hash_abort with SHA512 algorithm
[Check 4] Test psa_hash_finish after calling psa_hash_abort

TEST RESULT: PASSED

******************************************

TEST: 216 | DESCRIPTION: Testing crypto generator functions APIs | UT: psa_generate_key
[Info] Executing tests from non-secure
[Check 1] Test psa_generate_key 16 Byte AES
[Check 2] Test psa_generate_key 24 Byte AES
[Check 3] Test psa_generate_key 32 Byte AES
[Check 4] Test psa_generate_key with RSA 2048 Keypair
[Check 5] Test psa_generate_key with ECC KeyPair
[Check 6] Test psa_generate_key with RSA 2048 Public key
[Check 7] Test psa_generate_key with invalid key type
[Check 8] Test psa_generate_key with invalid usage flags

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
[Check 9] Test psa_key_derivation_input_key with key type as 2048 RSA public key
[Check 10] Test psa_key_derivation_input_key with key type as RSA 2048 keypair
[Check 11] Test psa_key_derivation_input_key with zero as step
[Check 12] Test psa_cipher_decrypt_setup - Invalid key handle
[Check 13] Test psa_cipher_decrypt_setup - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 219 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_key_derivation_key_agreement
[Info] Executing tests from non-secure
[Check 1] Test psa_key_derivation_key_agreement - ECDH SECP256R1
[Check 2] Test psa_key_derivation_key_agreement - Invalid step
[Check 3] Test psa_key_derivation_key_agreement - ECDH SECP384R1
[Check 4] Test psa_key_derivation_key_agreement - Invalid usage
[Check 5] Test psa_key_derivation_key_agreement - KDF not a key agreement alg
[Check 6] Test psa_key_derivation_key_agreement - Public key of different curve
[Check 7] Test psa_key_derivation_key_agreement - Pub key instead of Prv key
[Check 8] Test psa_key_derivation_key_agreement - Invalid handle
[Check 9] Test psa_key_derivation_key_agreement - Zero as handle

TEST RESULT: PASSED

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
[Check 6] Test psa_aead_encrypt - GCM - AES - 12B nonce & 12B add data
[Check 7] Test psa_aead_encrypt - Unsupported algorithm
[Check 8] Test psa_aead_encrypt - Invalid usage flag
[Check 9] Test psa_aead_encrypt - Invalid ciphertext_size
[Check 10] Test psa_aead_encrypt - Invalid nonce
[Check 11] Test psa_aead_encrypt - Invalid tag length 0

TEST RESULT: PASSED

******************************************

TEST: 225 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_decrypt
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_decrypt - CCM - AES - 13B nonce & 8B add data
[Check 2] Test psa_aead_decrypt - CCM - AES - 13B nonce & 32B add data
[Check 3] Test psa_aead_decrypt - CCM - AES - 24 bytes Tag length = 4
[Check 4] Test psa_aead_decrypt - CCM - AES - Zero additional data
[Check 5] Test psa_aead_decrypt - CCM - AES - Zero plaintext
[Check 6] Test psa_aead_decrypt - GCM - AES - 12B nonce & 12B add data
[Check 7] Test psa_aead_decrypt - Unsupported algorithm
[Check 8] Test psa_aead_decrypt - Invalid usage flag
[Check 9] Test psa_aead_decrypt - Invalid plaintext_size
[Check 10] Test psa_aead_decrypt - Invalid nonce
[Check 11] Test psa_aead_decrypt - Invalid cihpertext
[Check 12] Test psa_aead_decrypt - Invalid cihpertext_size
[Check 13] Test psa_aead_decrypt - Invalid tag length 0

TEST RESULT: PASSED

******************************************

TEST: 226 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_sign_setup
[Info] Executing tests from non-secure
[Check 1] Test psa_mac_sign_setup - HMAC - SHA256
[Check 2] Test psa_mac_sign_setup - CMAC - AES
[Check 3] Test psa_mac_sign_setup - Incompatible HMAC for CMAC
[Check 4] Test psa_mac_sign_setup - Invalid usage flag
[Check 5] Test psa_mac_sign_setup - Invalid key type
[Check 6] Test psa_mac_sign_setup - Truncated MAC too large
[Check 7] Test psa_mac_sign_setup - Truncated MAC too small
[Check 8] Test psa_mac_sign_setup - Unknown MAC algorithm
[Check 9] Test psa_mac_sign_setup - Bad algorithm (not a MAC algorithm)
[Check 10] Test psa_mac_sign_setup invalid key handle
[Check 11] Test psa_mac_sign_setup zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 227 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_update
[Info] Executing tests from non-secure
[Check 1] Test psa_mac_update - HMAC - SHA256 - 64 Byte
[Check 2] Test psa_mac_update - CMAC - AES - 16 Byte
[Check 3] Test psa_mac_update - HMAC - SHA512 - 32 Byte
[Check 4] Test psa_mac_update - HMAC - SHA512 - Invalid operation state

TEST RESULT: PASSED

******************************************

TEST: 228 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_sign_finish
[Info] Executing tests from non-secure
[Check 1] Test psa_mac_sign_finish  - HMAC - SHA256
[Check 2] Test psa_mac_sign_finish  - HMAC - SHA512
[Check 3] Test psa_mac_sign_finish  - CMAC - AES

TEST RESULT: PASSED

******************************************

TEST: 229 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_verify_setup
[Info] Executing tests from non-secure
[Check 1] Test psa_mac_verify_setup - HMAC - SHA256
[Check 2] Test psa_mac_verify_setup - CMAC - AES
[Check 3] Test psa_mac_verify_setup - Incompatible HMAC for CMAC
[Check 4] Test psa_mac_verify_setup - Invalid usage flag
[Check 5] Test psa_mac_verify_setup - Invalid key type
[Check 6] Test psa_mac_verify_setup - Truncated MAC too large
[Check 7] Test psa_mac_verify_setup - Truncated MAC too small
[Check 8] Test psa_mac_verify_setup - Unknown MAC algorithm
[Check 9] Test psa_mac_verify_setup - Bad algorithm (not a MAC algorithm)
[Check 10] Test psa_mac_verify_setup invalid key handle
[Check 11] Test psa_mac_verify_setup zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 230 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_verify_finish
[Info] Executing tests from non-secure
[Check 1] Test psa_mac_verify_finish - HMAC - SHA256
[Check 2] Test psa_mac_verify_finish - HMAC - SHA512
[Check 3] Test psa_mac_verify_finish - CMAC - AES

TEST RESULT: PASSED

******************************************

TEST: 231 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_abort
[Info] Executing tests from non-secure
[Check 1] Test psa_mac_abort - HMAC - SHA256
[Check 2] Test psa_mac_abort - HMAC - SHA512
[Check 3] Test psa_mac_abort - CMAC - AES
[Check 4] Test psa_mac_sign_finish after calling psa_mac_abort

TEST RESULT: PASSED

******************************************

TEST: 232 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_encrypt_setup
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_encrypt_setup 16 Byte AES
[Check 2] Test psa_cipher_encrypt_setup 24 Byte AES
[Check 3] Test psa_cipher_encrypt_setup 32 Byte AES
[Check 4] Test psa_cipher_encrypt_setup 16 Byte raw data
[Check 5] Test psa_cipher_encrypt_setup - not a cipher algorithm
[Check 6] Test psa_cipher_encrypt_setup - unknown cipher algorithm
[Check 7] Test psa_cipher_encrypt_setup - incorrect usage
[Check 8] Test psa_cipher_encrypt_setup - RSA public key
[Check 9] Test psa_cipher_encrypt_setup - RSA keypair
[Check 10] Test psa_cipher_encrypt_setup - EC Public key
[Check 11] Test psa_cipher_encrypt_setup - EC keypair
[Check 12] Test psa_cipher_encrypt_setup - Invalid key handle
[Check 13] Test psa_cipher_encrypt_setup - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 233 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_decrypt_setup
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_decrypt_setup 16 Byte AES
[Check 2] Test psa_cipher_decrypt_setup 24 Byte AES
[Check 3] Test psa_cipher_decrypt_setup 32 Byte AES
[Check 4] Test psa_cipher_decrypt_setup 16 Byte raw data
[Check 5] Test psa_cipher_decrypt_setup - not a cipher algorithm
[Check 6] Test psa_cipher_decrypt_setup - unknown cipher algorithm
[Check 7] Test psa_cipher_decrypt_setup - incorrect usage
[Check 8] Test psa_cipher_decrypt_setup - RSA public key
[Check 9] Test psa_cipher_decrypt_setup - RSA keypair
[Check 10] Test psa_cipher_decrypt_setup - EC Public key
[Check 11] Test psa_cipher_decrypt_setup - EC keypair
[Check 12] Test psa_cipher_decrypt_setup - Invalid key handle
[Check 13] Test psa_cipher_decrypt_setup - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 234 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_generate_iv
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_generate_iv 16 Byte AES
[Check 2] Test psa_cipher_generate_iv 24 Byte AES
[Check 3] Test psa_cipher_generate_iv 32 Byte AES
[Check 4] Test psa_cipher_generate_iv AES - small iv buffer

TEST RESULT: PASSED

******************************************

TEST: 235 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_set_iv
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_set_iv 16 Byte AES
[Check 2] Test psa_cipher_set_iv 24 Byte AES
[Check 3] Test psa_cipher_set_iv 32 Byte AES
[Check 4] Test psa_cipher_set_iv AES - small iv buffer
[Check 5] Test psa_cipher_set_iv AES - large iv buffer

TEST RESULT: PASSED

******************************************

TEST: 236 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_update
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_update - Encrypt - AES CBC_NO_PADDING
[Check 2] Test psa_cipher_update - Encrypt - AES CBC_NO_PADDING (Short in)
[Check 3] Test psa_cipher_update - Encrypt - AES CBC_PKCS7
[Check 4] Test psa_cipher_update - Encrypt - AES CBC_PKCS7 (Short input)
[Check 5] Test psa_cipher_update - Encrypt - AES CTR
[Check 6] Test psa_cipher_update - Encrypt - small output buffer size
[Check 7] Test psa_cipher_update - Decrypt - AES CBC_NO_PADDING
[Check 8] Test psa_cipher_update - Decrypt - AES CBC_NO_PADDING (Short in)
[Check 9] Test psa_cipher_update - Decrypt - AES CBC_PKCS7
[Check 10] Test psa_cipher_update - Decrypt - AES CBC_PKCS7 (Short input)
[Check 11] Test psa_cipher_update - Decrypt - AES CTR
[Check 12] Test psa_cipher_update - Decrypt - small output buffer size
[Check 13] Test psa_cipher_update without cipher setup

TEST RESULT: PASSED

******************************************

TEST: 237 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_finish
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_finish - Encrypt - AES CBC_NO_PADDING
[Check 2] Test psa_cipher_finish - Encrypt - AES CBC_NO_PADDING (Short in)
[Check 3] Test psa_cipher_finish - Encrypt - AES CBC_PKCS7
[Check 4] Test psa_cipher_finish - Encrypt - AES CBC_PKCS7 (Short input)
[Check 5] Test psa_cipher_finish - Encrypt - AES CTR
[Check 6] Test psa_cipher_finish - Encrypt - AES CTR (short input)
[Check 7] Test psa_cipher_finish - Encrypt - small output buffer size
[Check 8] Test psa_cipher_finish - Decrypt - AES CBC_NO_PADDING
[Check 9] Test psa_cipher_finish - Decrypt - AES CBC_NO_PADDING (Short in)
[Check 10] Test psa_cipher_update - Decrypt - AES CBC_PKCS7
[Check 11] Test psa_cipher_finish - Decrypt - AES CBC_PKCS7 (Short input)
[Check 12] Test psa_cipher_finish - Decrypt - AES CTR
[Check 13] Test psa_cipher_finish - Decrypt - AES CTR (short input)

TEST RESULT: PASSED

******************************************

TEST: 238 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_abort
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_abort - Encrypt - AES CBC_NO_PADDING
[Check 2] Test psa_cipher_abort - Encrypt - AES CBC_PKCS7
[Check 3] Test psa_cipher_abort - Encrypt - AES CTR
[Check 4] Test psa_cipher_abort - Decrypt - AES CBC_NO_PADDING
[Check 5] Test psa_cipher_abort - Decrypt - AES CBC_PKCS7
[Check 6] Test psa_cipher_abort - Decrypt - AES CTR
[Check 7] Test psa_cipher_update after psa_cipher_abort should fail

TEST RESULT: PASSED

******************************************

TEST: 239 | DESCRIPTION: Testing crypto asymmetric APIs | UT: psa_asymmetric_encrypt
[Info] Executing tests from non-secure
[Check 1] Test psa_asymmetric_encrypt - RSA PKCS1V15
[Check 2] Test psa_asymmetric_encrypt - RSA KEY_PAIR PKCS1V15
[Check 3] Test psa_asymmetric_encrypt - RSA OAEP SHA256
[Check 4] Test psa_asymmetric_encrypt - RSA OAEP SHA256 with label
[Check 5] Test psa_asymmetric_encrypt - RSA KEY_PAIR OAEP SHA256
[Check 6] Test psa_asymmetric_encrypt - RSA KEY_PAIR OAEP SHA256 with label
[Check 7] Test psa_asymmetric_encrypt - Small output buffer
[Check 8] Test psa_asymmetric_encrypt - Invalid algorithm
[Check 9] Test psa_asymmetric_encrypt - Invalid key type
[Check 10] Test psa_asymmetric_encrypt - Invalid usage
[Check 11] Test psa_asymmetric_encrypt - RSA PKCS1V15 - Salt
[Check 12] Test psa_asymmetric_encrypt - ECC public key
[Check 13] Test psa_asymmetric_encrypt - Invalid key handle
[Check 14] Test psa_asymmetric_encrypt - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 240 | DESCRIPTION: Testing crypto asymmetric APIs | UT: psa_asymmetric_decrypt
[Info] Executing tests from non-secure
[Check 1] Test psa_asymmetric_decrypt - RSA KEY_PAIR PKCS1V15
[Check 2] Test psa_asymmetric_decrypt - RSA KEY_PAIR OAEP SHA256
[Check 3] Test psa_asymmetric_decrypt - RSA KEY_PAIR OAEP SHA256 with label
[Check 4] Test psa_asymmetric_decrypt - Invalid key type (RSA public key)
[Check 5] Test psa_asymmetric_decrypt - Small output buffer
[Check 6] Test psa_asymmetric_decrypt - Invalid algorithm
[Check 7] Test psa_asymmetric_decrypt - Invalid key type (AES Key)
[Check 8] Test psa_asymmetric_decrypt - Invalid usage
[Check 9] Test psa_asymmetric_decrypt - RSA PKCS1V15 - Salt
[Check 10] Test psa_asymmetric_decrypt - Invalid key handle
[Check 11] Test psa_asymmetric_decrypt - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 241 | DESCRIPTION: Testing crypto asymmetric APIs | UT: psa_sign_hash
[Info] Executing tests from non-secure
[Check 1] Test psa_sign_hash - RSA KEY_PAIR PKCS1V15 RAW
[Check 2] Test psa_sign_hash - RSA KEY_PAIR PKCS1V15 SHA-256
[Check 3] Test psa_sign_hash - ECDSA SECP256R1 SHA-256
[Check 4] Test psa_sign_hash - Invalid key type (RSA public key)
[Check 5] Test psa_sign_hash - Small output buffer
[Check 6] Test psa_sign_hash - Invalid algorithm
[Check 7] Test psa_sign_hash - Invalid key type (AES Key)
[Check 8] Test psa_sign_hash - Invalid usage
[Check 9] Test psa_sign_hash - Wrong hash size
[Check 10] Test psa_sign_hash - Invalid key handle
[Check 11] Test psa_sign_hash - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 242 | DESCRIPTION: Testing crypto asymmetric APIs | UT: psa_verify_hash
[Info] Executing tests from non-secure
[Check 1] Test psa_verify_hash - RSA KEY_PAIR PKCS1V15 RAW
[Check 2] Test psa_verify_hash - RSA KEY_PAIR PKCS1V15 SHA-256
[Check 3] Test psa_verify_hash - ECDSA KEY_PAIR SECP256R1 SHA-256
[Check 4] Test psa_verify_hash - EC public key
[Check 5] Test psa_verify_hash - RSA public key
[Check 6] Test psa_verify_hash - Small output buffer
[Check 7] Test psa_verify_hash - Invalid algorithm
[Check 8] Test psa_verify_hash - Invalid key type (AES Key)
[Check 9] Test psa_verify_hash - Invalid usage
[Check 10] Test psa_verify_hash - Wrong hash size
[Check 11] Test psa_verify_hash - Wrong signature
[Check 12] Test psa_verify_hash - Wrong signature size
[Check 13] Test psa_verify_hash - Invalid key handle
[Check 14] Test psa_verify_hash - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 243 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_raw_key_agreement
[Info] Executing tests from non-secure
[Check 1] Test psa_raw_key_agreement - ECDH SECP256R1
[Check 2] Test psa_raw_key_agreement - Small buffer size
[Check 3] Test psa_raw_key_agreement - ECDH SECP384R1
[Check 4] Test psa_raw_key_agreement - Invalid usage
[Check 5] Test psa_raw_key_agreement - Unknown KDF
[Check 6] Test psa_raw_key_agreement - Not a key agreement alg
[Check 7] Test psa_raw_key_agreement - Public key on different curve
[Check 8] Test psa_raw_key_agreement - Public key instead of private key
[Check 9] Test psa_raw_key_agreement - Invalid key handle
[Check 10] Test psa_raw_key_agreement - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 244 | DESCRIPTION: Testing crypto key management APIs | UT: psa_copy_key
[Info] Executing tests from non-secure
[Check 1] Test psa_copy_key - 2048 RSA public key
[Check 2] Test psa_copy_key - RSA 2048 keypair
[Check 3] Test psa_copy_key - Incompatible target policy(source and target)
[Check 4] Test psa_copy_key - source key with no export usage
[Check 5] Test psa_copy_key - EC Public key
[Check 6] Test psa_copy_key - EC keypair

TEST RESULT: PASSED

******************************************

TEST: 245 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_clone
[Info] Executing tests from non-secure
[Check 1] Test psa_hash_clone - SHA256 algorithm
[Check 2] Test psa_hash_clone - SHA384 algorithm
[Check 3] Test psa_hash_clone - SHA512 algorithm
[Check 4] Test psa_hash_clone - from an inactive source operation
[Check 5] Test psa_hash_clone - on an active target operation

TEST RESULT: PASSED

******************************************

TEST: 246 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_compute
[Info] Executing tests from non-secure
[Check 1] Test psa_mac_compute HMAC SHA 256
[Check 2] Test psa_mac_compute HMAC SHA 512
[Check 3] Test psa_mac_compute CMAC AES 128
[Check 4] Test psa_mac_compute small size buffer
[Check 5] Test psa_mac_compute - Invalid key type

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
[Check 7] Test psa_mac_verify HMAC SHA 512
[Check 8] Test psa_mac_verify CMAC AES 128
[Check 9] Test psa_mac_verify - Invalid key type
[Check 10] Test psa_mac_verify small size buffer

TEST RESULT: PASSED

******************************************

TEST: 248 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_encrypt
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_encrypt - Encrypt - AES CBC_NO_PADDING
[Check 2] Test psa_cipher_encrypt - Encrypt - AES CBC_NO_PADDING (Short input)
[Check 3] Test psa_cipher_encrypt - Encrypt - AES CBC_PKCS7
[Check 4] Test psa_cipher_encrypt - Encrypt - AES CBC_PKCS7 (Short input)
[Check 5] Test psa_cipher_encrypt - Encrypt - AES CTR
[Check 6] Test psa_cipher_encrypt - Encrypt - AES CTR (short input)
[Check 7] Test psa_cipher_encrypt - small output buffer size
[Check 8] Test psa_cipher_encrypt - Decrypt - AES CBC_NO_PADDING

TEST RESULT: PASSED

******************************************

TEST: 249 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_decrypt
[Info] Executing tests from non-secure
[Check 1] Test psa_cipher_decrypt - Encrypt - AES CBC_NO_PADDING
[Check 2] Test psa_cipher_decrypt - Decrypt - AES CBC_NO_PADDING
[Check 3] Test psa_cipher_decrypt - Decrypt - AES CBC_NO_PADDING (Short input)
[Check 4] Test psa_cipher_decrypt - Decrypt - AES CBC_NO_PADDING
[Check 5] Test psa_cipher_decrypt - Decrypt - AES CBC_PKCS7
[Check 6] Test psa_cipher_decrypt - Decrypt - AES CBC_PKCS7 (Short input)
[Check 7] Test psa_cipher_decrypt - Decrypt - AES CTR

TEST RESULT: PASSED

******************************************

TEST: 250 | DESCRIPTION: Testing crypto key management APIs | UT: psa_sign_message
[Info] Executing tests from non-secure
[Check 1] Test psa_sign_message - RSA KEY_PAIR PKCS1V15 RAW
[Check 2] Test psa_sign_message - ECDSA SECP256R1 SHA-256
[Check 3] Test psa_sign_message - Invalid key type (RSA public key)
[Check 4] Test psa_sign_message - Small output buffer
[Check 5] Test psa_sign_message - Invalid algorithm
[Check 6] Test psa_sign_message - Invalid key type (AES Key)
[Check 7] Test psa_sign_message - Invalid usage
[Check 8] Test psa_sign_message - Mismatch key type and Algorithm
[Check 9] Test psa_sign_message - Alg mismatch in key and API call
[Check 10] Test psa_sign_message - Invalid key handle
[Check 11] Test psa_sign_message - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 251 | DESCRIPTION: Testing crypto key management APIs | UT: psa_verify_message
[Info] Executing tests from non-secure
[Check 1] Test psa_verify_message - RSA KEY_PAIR PKCS1V15 RAW
[Check 2] Test psa_verify_message - RSA Public Key PKCS1V15 RAW
[Check 3] Test psa_verify_message - RSA KEY_PAIR PKCS1V15 SHA-256
[Check 4] Test psa_verify_message - ECDSA KEY_PAIR SECP256R1 SHA-256
[Check 5] Test psa_verify_message - ECDSA Public Key SECP256R1 SHA-256
[Check 6] Test psa_verify_message - Small output buffer
[Check 7] Test psa_verify_message - Invalid algorithm
[Check 8] Test psa_verify_message - Invalid key type (AES Key)
[Check 9] Test psa_verify_message - Invalid usage
[Check 10] Test psa_verify_message - Wrong message size
[Check 11] Test psa_verify_message - Wrong signature
[Check 12] Test psa_verify_message - Wrong signature size
[Check 13] Test psa_verify_message - Invalid key handle
[Check 14] Test psa_verify_message - Zero as key handle

TEST RESULT: PASSED

******************************************

TEST: 252 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_encrypt_setup
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_encrypt_setup - CCM - AES
[Check 2] Test psa_aead_encrypt_setup - CCM - AES - Tag length = 4
[Check 3] Test psa_aead_encrypt_setup - CCM - AES - Mismatched tag length
[Check 4] Test psa_aead_encrypt_setup - CCM - AES - Default Tag length
[Check 5] Test psa_aead_encrypt_setup - GCM - AES
[Check 6] Test psa_aead_encrypt_setup - Invalid usage flag

TEST RESULT: PASSED

******************************************

TEST: 253 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_decrypt_setup
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_decrypt_setup - CCM - AES
[Check 2] Test psa_aead_decrypt_setup - CCM - AES - Tag length = 4
[Check 3] Test psa_aead_decrypt_setup - CCM - AES - Mismatched tag length
[Check 4] Test psa_aead_decrypt_setup - CCM - AES - Default Tag length
[Check 5] Test psa_aead_decrypt_setup - GCM - AES
[Check 6] Test psa_aead_decrypt_setup - Invalid usage flag

TEST RESULT: PASSED

******************************************

TEST: 254 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_generate_nonce
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_generate_nonce - CCM
[Check 2] Test psa_aead_generate_nonce - CCM - Tag length = 4
[Check 3] Test psa_aead_generate_nonce - CCM - Small buffer size
[Check 4] Test psa_aead_generate_nonce - CCM - Uninitialized operation
[Check 5] Test psa_aead_generate_nonce - CCM - Decrypt operation
[Check 6] Test psa_aead_generate_nonce - GCM
[Check 7] Test psa_aead_generate_nonce - GCM - Decrypt operation

TEST RESULT: PASSED

******************************************

TEST: 255 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_set_nonce
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_set_nonce - Encrypt - CCM
[Check 2] Test psa_aead_set_nonce - Encrypt - CCM - Tag length = 4
[Check 3] Test psa_aead_set_nonce - Encrypt - CCM - Small nonce size
[Check 4] Test psa_aead_set_nonce - Encrypt - CCM - Large nonce size
[Check 5] Test psa_aead_set_nonce - Encrypt - CCM - Invalid operation state
[Check 6] Test psa_aead_set_nonce - Encrypt - GCM
[Check 7] Test psa_aead_set_nonce - Decrypt - CCM
[Check 8] Test psa_aead_set_nonce - Decrypt - CCM - Tag length = 4
[Check 9] Test psa_aead_set_nonce - Decrypt - CCM - Small nonce size
[Check 10] Test psa_aead_set_nonce - Decrypt - CCM - Large nonce size
[Check 11] Test psa_aead_set_nonce - Decrypt - CCM - Invalid operation state
[Check 12] Test psa_aead_set_nonce - Decrypt - GCM

TEST RESULT: PASSED

******************************************

TEST: 256 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_set_lengths
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_set_lengths - Encrypt - CCM
[Check 2] Test psa_aead_set_lengths - Encrypt - CCM - Tag length = 4
[Check 3] Test psa_aead_set_lengths - Encrypt - CCM - Zero ad_length
[Check 4] Test psa_aead_set_lengths - Encrypt - CCM - Zero plaintext_length
[Check 5] Test psa_aead_set_lengths - Encrypt - CCM - Invalid operation state
[Check 6] Test psa_aead_set_lengths - Encrypt - GCM
[Check 7] Test psa_aead_set_lengths - Decrypt - CCM
[Check 8] Test psa_aead_set_lengths - Decrypt - CCM - Tag length = 4
[Check 9] Test psa_aead_set_lengths - Decrypt - CCM - Zero ad_length
[Check 10] Test psa_aead_set_lengths - Decrypt - CCM - Zero plaintext_length
[Check 11] Test psa_aead_set_lengths - Decrypt - CCM - Invalid operation state
[Check 12] Test psa_aead_set_lengths - Decrypt - GCM

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
[Check 5] Test psa_aead_finish - GCM - 16B AES - 12B Nonce & 12B additional data

TEST RESULT: PASSED

******************************************

TEST: 260 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_abort
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_abort - Encrypt - CCM - AES
[Check 2] Test psa_aead_abort - Encrypt - GCM - AES
[Check 3] Test psa_aead_abort - Decrypt - CCM - AES
[Check 4] Test psa_aead_abort - Decrypt - GCM - AES
[Check 5] Test psa_aead_abort with all initializations

TEST RESULT: PASSED

******************************************

TEST: 261 | DESCRIPTION: Testing crypto AEAD APIs | UT: psa_aead_verify
[Info] Executing tests from non-secure
[Check 1] Test psa_aead_verify - AES-CCM
[Check 2] Test psa_aead_verify - AES-CCM 24 bytes Tag length = 4
[Check 3] Test psa_aead_verify - Small buffer size
[Check 4] Test psa_aead_verify - Input length is less than plaintext length
[Check 5] Test psa_aead_verify - GCM - 16B AES - 12B Nounce & 12B addi data

TEST RESULT: PASSED

******************************************

************ Crypto Suite Report **********
TOTAL TESTS     : 61
TOTAL PASSED    : 61
TOTAL SIM ERROR : 0
TOTAL FAILED    : 0
TOTAL SKIPPED   : 0
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
UID 13 set failed due to insufficient space
Remove all registered UIDs
[Check 2] Overload storage again to verify all previous UID removed
UID 13 set failed due to insufficient space
Remove all registered UIDs

[Info] Executing PS tests
[Check 1] Overload storage space
UID 13 set failed due to insufficient space
Remove all registered UIDs
[Check 2] Overload storage again to verify all previous UID removed
UID 13 set failed due to insufficient space
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

TrustZone Application Development
----------------------------------------
Every TrustZone based application consists of two independent parts - secure part/project and non-secure part/project.

The secure project is stored in <application_name>\<application_name>_s directory.
The non-secure project is stored in <application_name>\<application_name>_ns directory. 

The secure projects always contains TrustZone configuration and it is executed after device RESET. The secure project usually
ends by jump to non-secure application/project.

TrustZone application compilation
--------------------------------
Please compile secure project firstly since CMSE library is needed for compilation of non-secure project.
After successful compilation of secure project, compile non-secure project.

TrustZone application debugging
-------------------------------
- Download both output file into device memory
- Start execution of secure project since secure project is going to be executed after device RESET.

Device header file and secure/non-secure access to the peripherals
------------------------------------------------------------------
Both secure and non-secure project uses identical device header file. The access to secure and non-secure aliases for all peripherals
is managed using compiler macro __ARM_FEATURE_CMSE.

For secure project using <PERIPH_BASE> means access through secure alias (address bit A28=1), 
using <PERIPH_BASE>_NS means access through non-secure alias(address bit A28=0)
For non-secure project using <PERIPH_BASE> means access through non-secure alias (address bit A28=0). 
The non-secure project doesn't have access to secure memory or peripherals regions so the secure access is not defined.


RW61x specific changes/adaptations of TF-M
==========================================

1. Use RW61x ROMAPI Flash driver for Flash memory writes
--------------------------------------------------------

A CMSIS Flash driver glue layer is added which does delegate Flash operations
to the ROMAPI Flash driver:

  * tf-m/platform/ext/target/nxp/common/CMSIS_Driver/Driver_Flash_iap_rw61x.c


2. Use OCOTP for nv rollback counters
--------------------------------------------------------

An implementation that uses OCOTP as backend for NV counters is available in:

  * tf-m/platform/ext/target/nxp/rdrw61x/nv_counters.c
  * tf-m/platform/ext/target/nxp/rdrw61x/platform_sp.c

A RAM emulation of the OTP fuses is provided. This emulation offers more fuses
than are available in OCOTP. This emulation is done for mainly two reasons:
  - during development/testing one does not want to make permanent changes to
    an IC
  - the available number of fuses available in OCOTP is very limited and
    therefore the max achievable counter/max achievable object writes is not
    big enough for the amount needed for all the TF-M tests to run.

The RAM emulation is enabled by defining preprocessor flag:

  * OCOTP_NV_COUNTERS_RAM_EMULATION 

This flag is enabled in the default SDK build. Building without the flag being
defined results in a build error for safety reasons (not to accidentially
enable OTP writes) and requires to remove an #error line from the respective
source file.


3. Use IPED and rollback protection for ITS service
--------------------------------------------------------

An additional Flash driver is introduced and used for ITS:

  * tf-m/platform/ext/target/nxp/common/CMSIS_Driver/Driver_Flash_iap_rw61x_iped.c

This driver makes use of the RW61x HW supported IPED. By using device specific
encryption keys, this binds the external Flash to the IC. In addition to
encryption, also rollback protection is introduced in this driver layer and
therefore a security level similar to an internal Flash.

For development/debugging encryption and rollback protection can be selectively
enabled/disabled with preprocessor defines. In the default SDK build, both features 
are enabled, one can set following options to enable/disable these:

  * RW61X_IPED_ENCRYPT_ENABLE : 1/0
  * RW61X_IPED_ITS_ROLLBACK_PROTECTION_ENABLE : 1/0


4. Use ELS/S50 keys as ROT for HUK and IAK
--------------------------------------------------------

It is possible to rely on NXP-provisioned data as ROT for HUK and IAK. The
RW61x boot ROM bootloader installs several keys in ELS keyslots that are usable
as ROT.

For HUK, the TF-M mechanism of built-in keys is reused. Upon startup a HUK is
derived from the DIE_INT_MK_SK (loaded by RW61x boot ROM) and stored in the
builtin_key_loader of TF-M. The TF-M default TF-M mechanisms for deriving sub
keys from this can then me used.

For IAK, no key is loaded to the builtin_key_loader. Instead, when an
attestation signature is done, a key is derived from NXP_DIE_EL2GOPUBLIC_MK_SK
(loaded by RW61x boot ROM) on the fly at the point in time the signature is
calculated. Only asymmetric attestation is supported by this mechanism.

Both of them can be individually be enabled or disabled with preprocessor
flags:

  * USE_ELS_PKC_HUK
  * USE_ELS_PKC_IAK
    
