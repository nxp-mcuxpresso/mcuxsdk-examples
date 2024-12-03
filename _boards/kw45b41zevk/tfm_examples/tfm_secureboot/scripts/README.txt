################################################################################
 Secure Boot - Getting Started
################################################################################

################################################################################
 Requirements
################################################################################
Hardware:
    - KW45B41Z-EVK
    - USB cable.

Software and Tools:
    - Windows 10 platform, to execute batch scripts.
    - IAR IDE v9.30.1 or higher.
    - MCUXpresso Secure Provisioning Tool (SEC) v6 (b220928) https://nxp.com/mcuxpresso/secure, installed to the default C:\nxp\MCUX_Provi_v6\bin folder.

Please follow the step by step guide how to do an initial provisioning, and following firmware (FW) updates.

################################################################################
0. Preparation.
################################################################################
    - Build the TFM Secure Boot example:
        - Open the "<example>\tfm_secureboot_s\iar\tfm_secureboot.eww" workspace in the IAR IDE.
        - Click on the "Batch Build" button in the "Project Toolbar". It starts building of the tfm_secureboot_s and tfm_secureboot_ns projects.
        - Wait until all builds processes are finished.
          NOTE: The output files, which will be used for the Firmware (FW) image creation:
                - "<example>\tfm_secureboot_s\mdk\release\tfm_secureboot_s.hex".
                - "<example>\tfm_secureboot_ns\mdk\release\tfm_secureboot_ns.hex".
    - Put device to the In System Programming (ISP) mode:
        - Press and hold the boot config switch SW4. It enables Auto ISP boot mode.
        - Connect a micro USB cable between the PC host and the micro-USB connector.
		- Release the switch SW4.
		- After that, the device boots into the UART HID ISP mode.
		- Determine serial port number of the connected device:
        - Open the Device Manager. Find the "LPC-LinkII UCom Port (COMx)" port number in the "Ports(COM & LPT)" folder. 
        - Update the COM port number in "<example>\scripts\config.cmd" file.

################################################################################
1. Provisioning.
################################################################################
    The provisioning should be performed only once.
    - Execute the "<example>\scripts\1_provision.bat" file.
      It performs a generation and a following loading of a provisioning SB file containing the boot Image.
    - The provisioning script will ask you: WARNING: Do you want to fuse ROTKTH and SB3KDK(irreversible operation) [N,Y]?
	  Possible answers:
        - Y = Write the fuses with ROTKTH and SB3KDK. It is irreversible.
              You have 10 seconds to answer Y, otherwise the default answer is N.
        - N = Fuses are not blown. Please note without these fuses blown, secure boot/authentication will fail.
	      The authentication failure will be seen once you have moved OEM LC to Closed state.
    - The provisioning script will ask you: " Do you want to write IFR0 - PRINCE Regions(irreversible operation) [N,Y]?"
	  Possible answers:
        - Y = Write the PRINCE configuration in IFR0 region. This is ir-reversible.
	      It would not be possible to change the configuration in IFR0 once written.
              By default System Lock and Global Lock is not enabled in the provided example for IFR0 - NPX configuration.
	      You have 10 seconds to answer Y, otherwise the default answer is N.
        - N = IFR0 PRINCE configuration is not written,
    - The provisioning scrip will ask you:  "Do you want to change the device Life Cycle (irreversible operation) [N,Y]?". 
      	  Possible answers:
        - Y = switch to the OEM_CLOSED state. It is irreversible. 
              You have 10 seconds to answer Y, otherwise the default answer is N.
        - N = do not change the life-cycle, so you can perform the provisioning many times.
              It is possible to switch the life-cycle later, by repeating the provisioning, or execute the "<example>\scripts\steps\1_5_seal_LC" step.
    - The command log:
		********************
		*** PROVISIONING ***
		********************
		### Check connection ###
		Response status = 0 (0x0) Success.
		Response word 1 = 1258488064 (0x4b030100)
		Current Version = K3.1.0
		### Flash erase. ###
		Response status = 0 (0x0) Success.
		SUCCESS
		### Check connection ###
		Response status = 0 (0x0) Success.
		Response word 1 = 1258488064 (0x4b030100)
		Current Version = K3.1.0
		### Merge S and NS images to one input\tfm_secureboot.bin file ###
		### Create bootable image ###
		nxpimage mbi export configs\mbi_config.json
		Success. (Master Boot Image: C:/_DDM/projects/mcu-sdk-2.0-k4w/boards/kw45b41zevk/tfm_examples/tfm_secureboot/scripts/spt_workspace/bootable_images/tfm_secureboot.bin created.)
		### Create IFR ROM configuration page - binary ###
		ifr generate-binary -d kw45xx -c "configs\romcfg.json" -o "configs\romcfg.bin"
		Success. (IFR binary has been generated into configs\romcfg.bin.)
		### Create SB file ###
		### Dump ROTKTH in a file ###
		SUCCESS
		### Provisioning the ROTKTH and SB3KDK is essential for further steps. ###
		### So we will EXIT provisioning if this step is not selected. ###
		### If the fuses are already provisioned say 'Y' to option to check if fuse values match. ###
		WARNING: Do you want to fuse the ROTKTH and SB3KDK (irreversible operation). [N,Y]?Y
		### Check connection ###
		Response status = 0 (0x0) Success.
		Response word 1 = 1258488064 (0x4b030100)
		Current Version = K3.1.0
		rotkth is 650d8097079ff27a3e8a2da14781b922fd8295b6c00bfa067f00e87f1a16b8b3
		sb3kdk is 7aa7ef9813b3561257b8837dab26225301df3511217f2733c71dadcd447722d1
		### Increase voltage for fuse burning ###
		Response status = 0 (0x0) Success.
		### Burn fuse: CUST_PROD_OEMFW_AUTH_PUK[0:255] ###
		blhost -p COM7,115200 -- fuse-program 0x1F {{650d8097079ff27a3e8a2da14781b922fd8295b6c00bfa067f00e87f1a16b8b3}}
		Response status = 0 (0x0) Success.
		Response word 1 = 32 (0x20)
		### Burn fuse: CUST_PROD_OEMFW_ENC_SK[0:255] ###
		blhost -p COM7,115200 -- fuse-program 0x20 {{7aa7ef9813b3561257b8837dab26225301df3511217f2733c71dadcd447722d1}}
		Response status = 0 (0x0) Success.
		Response word 1 = 32 (0x20)
		### Set voltage to normal value ###
		Response status = 0 (0x0) Success.
		### Reset ###
		Response status = 0 (0x0) Success.
		### Delay until processor is restarted; ###
		### Check connection ###
		Response status = 0 (0x0) Success.
		Response word 1 = 1258488064 (0x4b030100)
		Current Version = K3.1.0
		SUCCESS
		Press any key to continue . . .
		WARNING: Do you want to write IFR0 - PRINCE Regions(irreversible operation) [N,Y]?Y
		### Check connection ###
		Response status = 0 (0x0) Success.
		Response word 1 = 1258488064 (0x4b030100)
		Current Version = K3.1.0
		### Write IFR ROM configuration. romcfg.bin is generated at build time ###
		Writing memory  [####################################]  100%
		Response status = 10203 (0x27db) Memory Cumulative Write.
		### Reset ###
		Response status = 0 (0x0) Success.
		### Delay until processor is restarted; ###
		### Check connection ###
		Response status = 0 (0x0) Success.
		Response word 1 = 1258488064 (0x4b030100)
		Current Version = K3.1.0
		SUCCESS
		### Check connection ###
		Response status = 0 (0x0) Success.
		Response word 1 = 1258488064 (0x4b030100)
		Current Version = K3.1.0
		### Write OEM SB file ###
		blhost -p COM7,115200 -- receive-sb-file "bootable_images\tfm_secureboot.sb"
		Sending SB file  [####################################]  100%
		Response status = 0 (0x0) Success.
		SUCCESS
		WARNING: Do you want to change the device Life Cycle (irreversible operation) [N,Y]?Y
		### Check connection ###
		Response status = 0 (0x0) Success.
		Response word 1 = 1258488064 (0x4b030100)
		Current Version = K3.1.0
		### Increase voltage for fuse burning ###
		Response status = 0 (0x0) Success.
		### Burn fuse: LC_STATE[7:0] Switch to OEM Secure closed (0xf) state ###
		Response status = 10 (0x0) Success.
		### Set voltage to normal value ###
		Response status = 0 (0x0) Success.
		SUCCESS
		*** Provisioning is SUCCESSFUL ***
		Press any key to continue . . . 
    - Disable the ISP mode, and run the FW application according to the Chapter [3].
    NOTE:
          - To enable a secure debug authentication, it is essential to move LifeCycle to Closed. 
          - The executable image is placed in the "<example>\scripts\spt_workspace\input\tfm_secureboot.bin" file.
            It is created by merging the "tfm_secureboot_s.hex" and "tfm_secureboot_ns.hex" files and folowing converting to the binary file.
          - The Master Boot Image (MBI) configuration is located in the "<example>\scripts\spt_workspace\configs\mbi_config.json" file.
		  - The IFR0 configuration is located in the "<example>\scripts\spt_workspace\configs\romcfg.json"
			  - Configures the NPX to use PRINCE, Global Encryption and Global Decryption.
			  - Configures the PRINCE Regions. [0x00000000-0x000B01FF] to protect a FW confidentiality while the FW image is stored in the internal Flash.
          - The ROTKTH enables boot image authentication and integrity using ECDSA P-384 with SHA384.
	      - The self-signed RoT authentication keys are located in "<example>\scripts\spt_workspace\keys\ROT<x>_p384.pem". First ROT1_p384.pem is used by default.
	        Once the ROTKTH is blown in the fuses, to experiment with new keys you will have to either change Silicon or the board.

################################################################################
2. Update firmware (FW).
################################################################################
    It is optional, as the provisioned device already contains the initial version of FW.
    The FW update can be performed only on an already provisioned device.
    - The device must be in the ISP mode, see Chapter [0]. 
    - Execute the "<example>\scripts\2_update_fw.bat" file.
    - It performs generation and following loading of the OEM SB file containing the updated FW.
    - The command log:
		********************
		**** FW UPDATE *****
		********************
		### Merge S and NS images to one input\tfm_secureboot.bin file ###
		### Align bootable image size to PRINCE sub-region boundary ###
		C:\nxp\MCUX_Provi_v6\bin\tools\utils\win\align.exe "bootable_images\tfm_secureboot.bin" 721408
		File size aligned to 721408 bytes
		### Create bootable image ###
		nxpimage mbi export configs\mbi_config.json
		Success. (Master Boot Image: 
		C:/_DDM/projects/mcu-sdk-2.0-k4w/boards/kw45b41zevk/tfm_examples/tfm_secureboot/scripts/spt_workspace/bootable_images/tfm_secureboot.bin created.)
		### Create OEM SB file ###
		nxpimage sb31 export configs\sb3_config.json
		Success. (Secure binary 3.1: 
		C:/_DDM/projects/mcu-sdk-2.0-k4w/boards/kw45b41zevk/tfm_examples/tfm_secureboot/scripts/spt_workspace/bootable_images/tfm_secureboot.sb created.)
		SUCCESS
		### Check connection ###
		Response status = 0 (0x0) Success.
		Response word 1 = 1258488064 (0x4b030100)
		Current Version = K3.1.0
		### Update bootable image using SB file ###
		blhost -p COM7,115200 -- receive-sb-file "bootable_images\tfm_secureboot.sb"
		Sending SB file  [####################################]  100%
		Response status = 0 (0x0) Success.
		SUCCESS
		*** FW Update is SUCCESSFUL ***
		Press any key to continue . . .	
    - Disable the ISP mode and run the FW application according to the Chapter [3].
    NOTE:
        - The executable image is placed in the "<example>\scripts\spt_workspace\input\tfm_secureboot.bin" file.
          It is created by merging "tfm_secureboot_s.hex" and "tfm_secureboot_ns.hex" files, by aligning it to 0x38000 (PRINCE region end)
          and following converting to the binary file.
        - The Master Boot Image (MBI) configuration is located in the "<example>\scripts\spt_workspace\configs\mbi_config.json" file.
        - If the FW Secure Boot is failed, it goes to the ISP mode.

################################################################################
3. Run FW application.
################################################################################
    To perform a secure boot of the device:
    - Connect a micro USB cable between the PC host and the CMSIS DAP USB portconnector on the board.
    - Determine serial port number of the connected device:
        - Open the Device Manager. Find the "LPC-LinkII UCom Port (COMx)" port number in the "Ports(COM & LPT)" folder. 
        - Use the COM port number for a serial terminal application.
    - Open a serial terminal for the determined COM port.
    - After power up, the device preforms the secure boot of the uploaded FW.
    - The serial terminal log:
		[WRN] This device was provisioned with dummy keys. This device is NOT SECURE
		[Sec Thread] Secure image initializing!
		TF-M FP mode: Hardware
		Lazy stacking enabled
		Booting TF-M 1.5.0
		Non-Secure system starting...

		***** PSA Architecture Test Suite - Version 1.3 *****

		Running.. Crypto Suite
		******************************************

		TEST: 201 | DESCRIPTION: Testing crypto key management APIs | UT:  psa_crypto_in                                                                                                                                                             it
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

		TEST: 204 | DESCRIPTION: Testing crypto key management APIs | UT: psa_export_pub                                                                                                                                                             lic_key
		[Info] Executing tests from non-secure
		[Check 1] Test psa_export_public_key with EC Public key

		TEST RESULT: PASSED

		******************************************

		TEST: 205 | DESCRIPTION: Testing crypto key management APIs | UT: psa_destroy_ke                                                                                                                                                             y
		[Info] Executing tests from non-secure
		[Check 1] Test psa_destroy_key with EC Public key

		TEST RESULT: PASSED

		******************************************

		TEST: 208 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_key_deriva                                                                                                                                                             tion_setup
		[Info] Executing tests from non-secure
		[Check 1] Test psa_key_derivation_setup - ECDH + HKDF-SHA-256
		[Check 2] Test psa_key_derivation_setup - ECDH, unknown KDF
		[Check 3] Test psa_key_derivation_setup - bad key derivation algorithm
		[Check 4] Test psa_key_derivation_setup - Invalid Algorithm

		TEST RESULT: PASSED

		******************************************

		TEST: 210 | DESCRIPTION: Testing crypto key attributes APIs | UT: psa_key_attrib                                                                                                                                                             utes_set_get
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

		TEST: 212 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_updat                                                                                                                                                             e
		[Info] Executing tests from non-secure
		[Check 1] Test psa_hash_update with SHA256 algorithm
		[Check 2] Test psa_hash_update without hash setup
		[Check 3] Test psa_hash_update with completed opertaion handle

		TEST RESULT: PASSED

		******************************************

		TEST: 213 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_verif                                                                                                                                                             y
		[Info] Executing tests from non-secure
		[Check 1] Test psa_hash_verify with SHA256 algorithm
		[Check 2] Test psa_hash_verify with incorrect expected hash
		[Check 3] Test psa_hash_verify with incorrect hash length
		[Check 4] test psa_hash_verify with inactive & invalid operation handle

		TEST RESULT: PASSED

		******************************************

		TEST: 214 | DESCRIPTION: Testing crypto hash functions APIs | UT: psa_hash_finis                                                                                                                                                             h
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

		TEST: 216 | DESCRIPTION: Testing crypto generator functions APIs | UT: psa_gener                                                                                                                                                             ate_key
		[Info] Executing tests from non-secure
		[Check 1] Test psa_generate_key with invalid key type
		[Check 2] Test psa_generate_key with invalid usage flags

		TEST RESULT: PASSED

		******************************************

		TEST: 217 | DESCRIPTION: Testing crypto generation APIs | UT: psa_generate_rando                                                                                                                                                             m
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

		TEST: 218 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_key_deriva                                                                                                                                                             tion_input_key
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

		TEST: 219 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_key_deriva                                                                                                                                                             tion_key_agreement
		[Info] Executing tests from non-secure
		[Check 1] Test psa_key_derivation_key_agreement - KDF not a key agreement alg

		TEST RESULT: SKIPPED (Skip Code=0x2d)

		******************************************

		TEST: 222 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_key_deriva                                                                                                                                                             tion_abort
		[Info] Executing tests from non-secure
		[Check 1] Test psa_key_derivation_abort

		TEST RESULT: PASSED

		******************************************

		TEST: 223 | DESCRIPTION: Testing crypto key derivation APIs | UT: psa_key_deriva                                                                                                                                                             tion_set_get_capacity
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

		TEST: 231 | DESCRIPTION: Testing crypto MAC APIs | UT: psa_mac_abort
		[Info] Executing tests from non-secure
		[Check 1] Test psa_mac_abort - HMAC - SHA256
		[Check 2] Test psa_mac_sign_finish after calling psa_mac_abort

		TEST RESULT: PASSED

		******************************************

		TEST: 232 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_e                                                                                                                                                             ncrypt_setup
		[Info] Executing tests from non-secure
		[Check 1] Test psa_cipher_encrypt_setup - unknown cipher algorithm
		[Check 2] Test psa_cipher_encrypt_setup - EC Public key
		[Check 3] Test psa_cipher_encrypt_setup - Invalid key handle
		[Check 4] Test psa_cipher_encrypt_setup - Zero as key handle

		TEST RESULT: PASSED

		******************************************

		TEST: 233 | DESCRIPTION: Testing crypto symmetric cipher APIs | UT: psa_cipher_d                                                                                                                                                             ecrypt_setup
		[Info] Executing tests from non-secure
		[Check 1] Test psa_cipher_decrypt_setup - unknown cipher algorithm
		[Check 2] Test psa_cipher_decry
		pt_setup - EC Public key
		[Check 3] Test psa_cipher_decrypt_setup - Invalid key handle
		[Check 4] Test psa_cipher_decrypt_setup - Zero as key handle

		TEST RESULT: PASSED

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

		TEST RESULT: PASSED

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

		************ Crypto Suite Report **********
		TOTAL TESTS     : 30
		TOTAL PASSED    : 27
		TOTAL SIM ERROR : 0
		TOTAL FAILED    : 0
		TOTAL SKIPPED   : 3
		******************************************

		Entering standby..

################################################################################
4. Debug FW application.
################################################################################
    To debug a firmware:
    - To run secure debug authentication on the device, change the lifecycle to OEM Secure Closed/OEM Closed. If you have not done it in the provisioning step,
      running the script below would provide you an option to do so. Do remember it is irreversible operation.
    - Run the FW application. Follow steps described in the Chapter 3.
    - Execute the "<example>\scripts\3_open_debug_port.bat" file, to perform a debug authentication and open the debug port on a device.
    - The command log: [ Log has N in device Life Cycle step as the LC is already changed)
		*********************
		** Open Debug Port **
		*********************
		Life cycle has to be OEM Secure Closed/Closed for Debug authentication to function
		### Debug Authentication ###
		"C:\nxp\MCUX_Provi_v6\bin\tools\spsdk\nxpdebugmbox.exe" -p 2.1 -i pyocd -s UNCEWCZ3KXW2X auth -b 0 -c debug_auth\debug_auth_cert.dc" -k "keys\debug_authentication_key_ECC_384.pem"
		  #   Interface   Id              Description
		------------------------------------------------------------------------------------------------
		  0   PyOCD       2LIZELYOP0EMV   NXP Semiconductors MCU-LINK on-board (r0C3) CMSIS-DAP V2.250
		  #   Interface   Id              Description
		------------------------------------------------------------------------------------------------
		  0   PyOCD       2LIZELYOP0EMV   NXP Semiconductors MCU-LINK on-board (r0C3) CMSIS-DAP V2.250
		The device is accessible for debugging.
		SUCCESS
		*** Debug Port Opening is SUCCESSFUL ***
		Press any key to continue . . .
    NOTE:
	  - Debug Authentication won't work till the life-cycle is changes to OEM Secure Close (0xF) or OEM Close (0x1F).
	    This is provided as an option in provisioning step in Chapter 1. If you have skipped that, debug authentication won't work.
	    Run script steps/1_5_seal_LC.bat to change the LC to 0xF. You can edit this script if you want to transition to 0x1F LC.
	    See RM for details.
          - The Debug Authentication Certificate is located in "<example>\scripts\spt_workspace\debug_auth\debug_auth_cert.dc".
          - The Debug Authentication Key is located in "<example>\scripts\spt_workspace\eys\debug_authentication_key_ECC_384.pem".

################################################################################
 Appendix.
################################################################################
- The scripts command configuration parameters are placed in the <example>\tfm_secureboot\scripts\config.cmd configuration file.
- The MCUXpresso Secure Provisioning Tool workspace is located in the "<example>\spt_workspace" folder, it can be used to change the PRINCE and MBI configurations.
- Useful documentation:
    - The Secure Provisioning SDK (SPSDK) user documentation: http://spsdk.nxp.com/index.html
    - The KW45B41Z Reference Manual, security chapters.

*Copyright 2023, NXP. All rights reserved.*
