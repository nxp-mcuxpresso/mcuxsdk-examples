################################################################################
 Secure Boot - Getting Started
################################################################################

################################################################################
 Requirements
################################################################################
Hardware:
    - RDRW612BGA-IPA-2A/1A V 2.1 EVK OR FRDMRW612 EVK, with the RW61X A1 chip.
    - USB cable.

Software and Tools:
    - Windows 10 platform, to execute batch scripts.
    - MCUXpresso IDE v11.7.1 [Build 9221] [2023-03-28]. 
    - MCUXpresso Secure Provisioning Tool (SEC) v6 https://nxp.com/mcuxpresso/secure, installed to the default "C:\Users\nxf84544\code\spsdk_rw61x_branch_merge_spsdk_1.9.1\venv" folder.

Please follow the step by step guide how to do an initial provisioning, and following firmware (FW) updates.

################################################################################
0. Preparation.
################################################################################
    - Build the TFM Secure Boot example by loading the sdk in MCUX and "Import SDK examples". 
	    - Select board and then from drop down menu, select tfm_secureboot_s and tfm_secureboot_ns examples:
        - Make sure, "copy sources" option is selected. Press Finish.
		- Choose Release Builds for both tfm_secureboot_s and tfm_secureboot_ns and build both projects.
        - Wait until all builds processes are finished.
          NOTE: The output files, which will be used for the Firmware (FW) image creation:
                - "<mcux_workspace>\<project name prefix>_tfm_secureboot_s\Release\tfm_secureboot_s.bin".
                - "<mcux_workspace>\<project name prefix>_tfm_secureboot_ns\Release\tfm_secureboot_ns.bin".
		- Once all builds are done, please do the following updates at path: "<mcux_workspace>\<project name prefix>_tfm_secureboot_s\<board>\tfm_examples\tfm_secureboot"
            - In file :"\tfm_secureboot\scripts\config.cmd"
		        - COM_PORT: e.g: COM_PORT=20
		        - TFM_EXAMPLE_S: path required for tfm_secureboot_s.bin file. e.g: SET "TFM_EXAMPLE_S=%TFM_EXAMPLE%..\..\..\Release\tfm_secureboot_s.bin"
			    - TFM_EXAMPLE_NS: path required for tfm_secureboot_ns.bin file. e.g: SET "TFM_EXAMPLE_NS=%TFM_EXAMPLE%..\..\..\..\frdmrw612_tfm_secureboot_ns\Release\tfm_secureboot_ns.bin"
			    - SPT_INSTALL_BIN: path for SPSDK installation directory: SET "SPT_INSTALL_BIN=C:\Users\nxf84544\code\spsdk_rw61x_branch\rwvenv\Scripts"
				
			- In files: "tfm_secureboot\scripts\spt_workspace\configs\sb3_config.yaml" and "tfm_secureboot\scripts\spt_workspace\config\sb3_devhsm_config.yaml"
			    - Configure the IPED regions  files as per instructions present in IPED section.
			        - IPED Configs: NOTE ABOUT END ADDRESS: There are two constraints: 
                      1- The application must fit into the region
                      2- The size of the region must be an integer multiple of 4 flash pages = 0x400 bytes
                    - So, Following needs to be done: Take last address from bootable_images\tfm_secureboot.bin, then round off the result via 0x400 and add the start IPED start address.
                    - e.g: in working example: last address: 0xC7F40, round off via ox400 => 0xC8000 + 0x08001000(Flash start address/IPED Start address) = 0x080C9000

################################################################################
1. Provisioning.
################################################################################
    The provisioning should be performed only once.
    - Put the device to the In System Programming (ISP) mode:
        - Configure the U38 pins to boot device into ISP boot mode.
        - Connect a micro USB cable between the PC host and the USB UART J21 port.
    - Execute the "<mcux_workspace>\<project name prefix>_tfm_secureboot_s\<board>\tfm_examples\tfm_secureboot\scripts\1_provision.bat" file.
      It performs a generation and a following loading of a provisioning SB file containing a CUST_MK_SK key, RKTH and Secure_Boot Enable configurations, a MB Signed Image.
        NOTE: This is an irreversible operation as CUST_MK_SK key, RKTH and Secure_Boot Enable configurations are written into the device OTPs. Please make sure you are ready to proceed with it.
      - The provisioning script will ask you:  "WARNING: Do you want to change the CUST_SK_MK, RKTH and SECURE_BOOT_EN OTPs in Device (irreversible operation) [N,Y]?". 
      Possible answers:
        - Y = Do the device initial provisining with CUST_MK_SK key, RKTH and Secure_Boot Enable configurations. It is irreversible. 
              You have 10 seconds to answer Y, otherwise the default answer is N.
        - N = do not do the initial provisining, so you can change the provisioning configurations
    - The provisioning script will ask you:  "Do you want to change the device Life Cycle (irreversible operation) [N,Y]?". 
      Possible answers:
        - Y = switch to the OEM_CLOSED (In-field) state. It is irreversible. 
              You have 10 seconds to answer Y, otherwise the default answer is N.
        - N = do not change the life-cycle, so you can perform the provisioning many times.
              It is possible to switch the life-cycle later, by repeating the provisioning, or execute the "<mcux_workspace>\<project name prefix>_tfm_secureboot_s\<board>\tfm_examples\tfm_secureboot\scripts\1_3_seal_irreversible.bat" step.
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
		Response status = 0 (0x0) Success.
		Response status = 0 (0x0) Success.
		SUCCESS
		### Check connection ###
		Response status = 0 (0x0) Success.
		Response word 1 = 1258488064 (0x4b030100)
		Current Version = K3.1.0
		### Merge S and NS images to one input\tfm_secureboot.bin file ###
		Success. (Extracted chunk: C:\Users\nxf84544\Documents\MCUXpressoIDE_11.7.1_9221\workspace\frdmrw612_tfm_secureboot_s\frdmrw612\tfm_examples\tfm_secureboot\scripts\spt_workspace\input\tfm_secureboot_s_stripped.bin created.)
		C:\Users\nxf84544\Documents\MCUXpressoIDE_11.7.1_9221\workspace\frdmrw612_tfm_secureboot_s\frdmrw612\tfm_examples\tfm_secureboot\scripts\..\..\..\..\..\frdmrw612_tfm_secureboot_ns\Release\tfm_secureboot_ns.bin
		1 File(s) copied
		Success. (Merged image: C:\Users\nxf84544\Documents\MCUXpressoIDE_11.7.1_9221\workspace\frdmrw612_tfm_secureboot_s\frdmrw612\tfm_examples\tfm_secureboot\scripts\spt_workspace\input\tfm_secureboot.bin created.)
		###Check if file created - input\tfm_secureboot.bin  ###
		### Create bootable image ###
		nxpimage mbi export configs\mbi_config.yaml
		Success. (Master Boot Image: C:/Users/nxf84544/Documents/MCUXpressoIDE_11.7.1_9221/workspace/frdmrw612_tfm_secureboot_s/frdmrw612/tfm_examples/tfm_secureboot/scripts/spt_workspace/bootable_images/tfm_secureboot.bin created.)
		### Create Provisioning SB file [CUST-MK-SK, RKTH, SECURE_BOOT_EN] ###
		nxpdevhsm generate -p COM20 -k configs\key_rw61x.bin -o configs\sb_seed.bin -j configs\sb3_devhsm_config.yaml -f rw61x bootable_images\dev_hsm_provisioning.sb3
		 1: Resetting the target device
		 2: Generating OEM master share.
		 3: Generating 48 bytes FW signing keys.
		 4: Generating 48 bytes FW encryption keys.
		 5: Wrapping CUST_MK_SK key.
		 6: Creating template un-encrypted SB3 header and data blobs.
		 6.1: Creating template SB3 header.
		 6.2: Creating un-encrypted SB3 data.
		 7: Encrypting SB3 data on device
		 7.1: Enriching encrypted SB3 data by mandatory hashes.
		 7.2: Creating dummy certificate block.
		 7.3: Updating SB3 header by valid values.
		 7.4: Preparing SB3 manifest to sign.
		 8: Creating SB3 manifest signature on device.
		 9: Composing final SB3 file.
		10: Resetting the target device
		Final SB3 file has been written: C:\Users\nxf84544\Documents\MCUXpressoIDE_11.7.1_9221\workspace\frdmrw612_tfm_secureboot_s\frdmrw612\tfm_examples\tfm_secureboot\scripts\spt_workspace\bootable_images\dev_hsm_provisioning.sb3
		SUCCESS
		WARNING: Do you want to change the CUST_SK_MK, RKTH and SECURE_BOOT_EN OTPs in Device (irreversible operation) [N,Y]?Y
		### Check connection ###
		Response status = 0 (0x0) Success.
		Response word 1 = 1258488064 (0x4b030100)
		Current Version = K3.1.0
		### Write Provisioning SB file ###
		blhost -p COM20 -t 300000 -- receive-sb-file bootable_images\dev_hsm_provisioning.sb3
		Sending SB file  [####################################]  100%
		Response status = 0 (0x0) Success.
		### Reset the processor, so the CUST_MK_SK key is applied ###
		Response status = 0 (0x0) Success.
		SUCCESS
        WARNING: Do you want to change the device Life Cycle (irreversible operation) [N,Y]?Y
        ### Check connection ###
        Response status = 0 (0x0) Success.
        Response word 1 = 1258488064 (0x4b030100)
        Current Version = K3.1.0
        ### Burn fuse: LC_STATE[15:0] Switch to OEM-Closed (0xf0f0) state ###
        Response status = 0 (0x0) Success.
        Response word 1 = True (0x1)
        SUCCESS
        *** Provisioning is SUCCESSFUL ***
        Press any key to continue . . .
    - Run the FW application according to the Chapter [3].
    NOTE:
          - To enable a secure debug authentication, update the CFPA area according to the Chapter [3]. 
          - The executable image is placed in the "<example>\scripts\spt_workspace\input\tfm_psatest.bin" file.
            It is created by merging the "tfm_secure_boot_s.hex" and "tfm_secure_boot_ns.hex" files, by aligning it to 0x38000 (PRINCE region end) and folowing converting to the binary file.
          - The self-signed RoT authentication keys are located in "<example>\scripts\spt_workspace\keys\ROT<x>_p384.pem". First ROT1_p384.pem is used by default. 
          - The Customer Manufacturing Configuration Area (CMPA) configuration is located in the "<example>\scripts\spt_workspace\configs\cmpa.json" file:
             - Enables boot image authentication and integrity using ECDSA P-384 with SHA384.
             - Enables PRINCE-based memory encryption for [0x00000000-0x00037FFF] memory region to protect a FW confidentiality while the FW image is stored in the internal Flash.
             - Disables the Debug Interface.
             - Enforces CMAC checking of images (SECURE_BOOT_CFG[31:30] = 0b01 in the PFR).
               Even if this bit field is set during first boot of the application image,
               the ECDSA algorithm is always used to verify authenticity of the image. After successful authentication,
               the bootloader calculates CMAC over the whole image. Device specific CMAC key is used.
               Calculated CMAC is then stored in Protected Flash Region (PFR), so that it can be used for authentication during next boot.
         - The Customer In-field Programmable Area (CFPA) configuration is located in the "<example>\scripts\spt_workspace\configs\cfpa.json" file.
         - The Master Boot Image (MBI) configuration is located in the "<example>\scripts\spt_workspace\configs\mbi_config.json" file.
         - The CUST_MK_SK master key, to derive FW image encryption key, located in the "<example>\scripts\spt_workspace\cust_mk_sk.bin" file.
         - The OEM share input, to use as a seed to randomize the provisioning process, located in the "<example>\scripts\spt_workspace\sb_seed.bin" file.

################################################################################
2. Update firmware (FW).
################################################################################
    It is optional, as the provisioned device already contains the initial version of FW.
    The FW update can be performed only on an already provisioned device.
    - Put the device to the In System Programming (ISP) mode:
        - Configure the U38 pins to boot device into ISP boot mode.
        - Connect a micro USB cable between the PC host and the USB UART J21 port.
    - Execute the "<mcux_workspace>\<project name prefix>_tfm_secureboot_s\<board>\tfm_examples\tfm_secureboot\scripts\2_update_fw.bat" file.
    - It performs generation and following loading of the OEM SB file containing the updated FW.
    - The command log:
		********************
		**** FW UPDATE *****
		********************
		### Merge S and NS images to one input\tfm_secureboot.bin file ###
		Success. (Extracted chunk: C:\Users\nxf84544\Documents\MCUXpressoIDE_11.7.1_9221\workspace\frdmrw612_tfm_secureboot_s\frdmrw612\tfm_examples\tfm_secureboot\scripts\spt_workspace\input\tfm_secureboot_s_stripped.bin created.)
		C:\Users\nxf84544\Documents\MCUXpressoIDE_11.7.1_9221\workspace\frdmrw612_tfm_secureboot_s\frdmrw612\tfm_examples\tfm_secureboot\scripts\..\..\..\..\..\frdmrw612_tfm_secureboot_ns\Release\tfm_secureboot_ns.bin
		1 File(s) copied
		Success. (Merged image: C:\Users\nxf84544\Documents\MCUXpressoIDE_11.7.1_9221\workspace\frdmrw612_tfm_secureboot_s\frdmrw612\tfm_examples\tfm_secureboot\scripts\spt_workspace\input\tfm_secureboot.bin created.)
		###Check if file created - input\tfm_secureboot.bin  ###
		### Create bootable image ###
		nxpimage mbi export "configs\mbi_config.yaml"
		Success. (Master Boot Image: C:/Users/nxf84544/Documents/MCUXpressoIDE_11.7.1_9221/workspace/frdmrw612_tfm_secureboot_s/frdmrw612/tfm_examples/tfm_secureboot/scripts/spt_workspace/bootable_images/tfm_secureboot.bin created.)
		### Create OEM SB file ###
		nxpimage sb31 export configs\sb3_config.yaml
		Success. (Secure binary 3.1: C:/Users/nxf84544/Documents/MCUXpressoIDE_11.7.1_9221/workspace/frdmrw612_tfm_secureboot_s/frdmrw612/tfm_examples/tfm_secureboot/scripts/spt_workspace/bootable_images/tfm_secureboot.sb3 created.)
		SUCCESS
		### Check connection ###
		Response status = 0 (0x0) Success.
		Response word 1 = 1258488064 (0x4b030100)
		Current Version = K3.1.0
		### Update bootable image using SB file ###
		"C:\Users\nxf84544\code\spsdk_rw61x_branch_merge_spsdk_1.9.1\venv\tools\spsdk\blhost.exe" -p COM20 -t240000 -- receive-sb-file "bootable_images\tfm_secureboot.sb3"
		Sending SB file  [####################################]  100%
		Response status = 0 (0x0) Success.
		SUCCESS
		*** FW Update is SUCCESSFUL ***
		Press any key to continue . . .
    - Run the FW application according to the Chapter [3].
    NOTE:
        - The executable image is placed in the "<example>\scripts\spt_workspace\input\tfm_psatest.bin" file.
          It is created by merging "tfm_secure_boot_s.hex" and "tfm_secure_boot_ns.hex" files, by aligning it to 0x38000 (PRINCE region end)
          and following converting to the binary file.
        - The Master Boot Image (MBI) configuration is located in the "<example>\scripts\spt_workspace\configs\mbi_config.json" file.
        - If the FW Secure Boot is failed, it goes to the ISP mode.

################################################################################
3. Run FW application.
################################################################################
    To perform a secure boot of the device:
    - Disable the ISP mode.
    - Connect a micro USB cable between the PC host and the J-Link/J-Trace USB UART port Link2 J21 connector on the board.
    - Determine serial port number of the connected device:
        - Open the Device Manager. Find the "USB serial Port (COMx)" port number in the "Ports(COM & LPT)" folder. 
        - Use the COM port number for a serial terminal application.
    - Open a serial terminal for the determined COM port.
    - After power up, the device preforms the secure boot of the uploaded FW.
    - The serial terminal log:
		[INF] Beginning TF-M provisioning
		[WRN] TFM_DUMMY_PROVISIONING is not suitable for production! This device is NOT SECURE
		[Sec Thread] Secure image initializing!
		TF-M Float ABI: Hard
		Lazy stacking enabled
		Booting TF-M 1.7.0
		sector version is too big for rollback counter: 0x00000000
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
		[Check 1] Test psa_raw_key_agreement - Not a key agreement alg

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
		TOTAL PASSED    : 60
		TOTAL SIM ERROR : 0
		TOTAL FAILED    : 0
		TOTAL SKIPPED   : 1
		******************************************

		Entering standby..



################################################################################
4. Debug FW application.
################################################################################
    To debug the firmware:
    - Run the FW application. Follow steps described in the Chapter 4.
    - Execute the "<mcux_workspace>\<project name prefix>_tfm_secureboot_s\<board>\tfm_examples\tfm_secureboot\scripts\3_open_debug_port.bat" file, to perform a debug authentication and open the debug port on a device.
    - The command log:
		*********************
		** Open Debug Port **
		*********************
		  #   Interface   Id           Description
		-------------------------------------------------------------------
		  0   Jlink       1064162557   Segger J-Link MCU-Link: 1064162557
		The device is accessible for debugging.
		### loading shadow register contents into device ram to simulate OTP behavior ###
		  #   Interface   Id           Description
		-------------------------------------------------------------------
		  0   Jlink       1064162557   Segger J-Link MCU-Link: 1064162557
		The Shadow registers has been loaded by configuration in C:\Users\nxf84544\Documents\MCUXpressoIDE_11.7.1_9221\workspace\frdmrw612_tfm_secureboot_s\frdmrw612\tfm_examples\tfm_secureboot\scripts\spt_workspace\debug_auth\filled_template_rw61x_shadow_config.yml YAML file
		  #   Interface   Id           Description
		-------------------------------------------------------------------
		  0   Jlink       1064162557   Segger J-Link MCU-Link: 1064162557
		The target has been reset.
		### resetting the device ###
		  #   Interface   Id           Description
		-------------------------------------------------------------------
		  0   Jlink       1064162557   Segger J-Link MCU-Link: 1064162557
		SUCCESS
		### Debug Authentication ###
		nxpdebugmbox  -i jlink --operation-timeout 20000 --protocol 2.0  auth --beacon 1 --certificate "debug_auth\debug_auth_cert.dc" --key "keys\debug_authentication_key_ECC_256.pem"
		  #   Interface   Id           Description
		-------------------------------------------------------------------
		  0   Jlink       1064162557   Segger J-Link MCU-Link: 1064162557
		  #   Interface   Id           Description
		-------------------------------------------------------------------
		  0   Jlink       1064162557   Segger J-Link MCU-Link: 1064162557
		The device is accessible for debugging.
		SUCCESS
		*** Debug Port Opening is SUCCESSFUL ***
		Press any key to continue . . .

    - Attach a debugger to the running device and debug it. The debugger starts, and the target hardware stops at the current location of the program counter.
        - For MCUX IDE:
            - Disable all of the following project options for a target:
                - Options For Target -> Debug - Load Application at startup.
                - Options For Target -> Debug -> Settings - Reset after connect.
                - Options For Target -> Utilities - Update Target before Debugging.
             - Start a Debug Session. The debugger starts without the application loading and the hardware reset.
    NOTE:
          - If a device life-cycle has not been updated in OTP during provisioning, to test the debug authentication, the script is trying to update the life-cycle register.
          - The Debug Authentication Certificate is located in "<mcux_workspace>\<project name prefix>_tfm_secureboot_s\<board>\tfm_examples\tfm_secureboot\scripts\spt_workspace\debug_auth\debug_auth_cert.dc".
          - The Debug Authentication Key is located in "<mcux_workspace>\<project name prefix>_tfm_secureboot_s\<board>\tfm_examples\tfm_secureboot\scripts\spt_workspace\eys\debug_authentication_key_ECC_256.pem".

################################################################################
5. Factory mass erase.
################################################################################
    To do a mass erase of the whole flash memory:
    - Put the device to the In System Programming (ISP) mode:
        - Configure the U38 pins to boot device into ISP boot mode.
        - Connect a micro USB cable between the PC host and the USB UART J21 port.
    - Execute the "workspace\<project name prefix>_tfm_secureboot_s\<board>\tfm_examples\tfm_secureboot\scripts\4_mass_erase.bat" file.
    - The command log:
		********************
		* FLASH MASS ERASE *
		********************
		### Check connection ###
		Response status = 0 (0x0) Success.
		Response word 1 = 1258488064 (0x4b030100)
		Current Version = K3.1.0
		### Flash erase. ###
		Response status = 0 (0x0) Success.
		Response status = 0 (0x0) Success.
		Response status = 0 (0x0) Success.
		SUCCESS
		*** Flash mass erase is SUCCESSFUL ***
		Press any key to continue . . .
    NOTE:
        - The Flash mass erase is available only if the device life-cycle has not been updated in OTP during the provisioning phase.

################################################################################
 Appendix.
################################################################################
- The scripts command configuration parameters are placed in the workspace\<project name prefix>_tfm_secureboot_s\<board>\tfm_examples\tfm_secureboot\scripts\config.cmd configuration file.
- The MCUXpresso Secure Provisioning Tool workspace is located in the "workspace\<project name prefix>_tfm_secureboot_s\<board>\tfm_examples\tfm_secureboot\scripts\spt_workspace" folder, it can be used to change IPED and MBI configurations.
- Useful documentation:
    - The Secure Provisioning SDK (SPSDK) user documentation: http://spsdk.nxp.com/index.html
    - The RW61x Reference Manual, security chapters:
        - Secure Boot ROM.
        - Security features.
        - Life Cycle States.
        - Prince.
        - Intrusion and Tamper Response Controller (ITRC).
        - Crypto Sub System (CSS).
        - Public-Key Crypto Coprocessor (PKC).
        - Trusted Execution Environment
        - Physically Unclonable Function (PUF).
        - Code Watchdog Timer (CDOG).

*Copyright 2022-2023, NXP. All rights reserved.*
