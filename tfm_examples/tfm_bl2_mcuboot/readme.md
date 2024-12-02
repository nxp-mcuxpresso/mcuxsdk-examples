# tfm_bl2_demo

## Overview
MCUboot is a system bootloader based on static allocation of the flash. It divides flash into
primary slot, where the active runtime images are stored and secondary slot where the candidate image
is written by the update client. MCUboot validates the image (hash + signature) and (if valid) swap
the image into the primary slot.

The Trusted Firmware M (TF-M) demo application provides a sanity check for the TF-M core and RoT services.
It can be used as a template for a new application which requires functionality of the TF-M Secure Services.

NOTE: The BL2 and TF-M main() functions have a non-standard location:
	   - Bootloader main() is in the bl2\ext\mcuboot\bl2_main.c file
       - Secure main() is in the tf-m\secure_fw\spm\cmsis_psa\main.c file.
       - Non-Secure main() is in the tf-m-tests\app\main_ns.c file.

Software requirements
---------------------
- Python 3.x
	- cbor>=1.0.0
	- click
	- cryptography
	- pyasn1
	- graphviz
	- imgtool>=1.6.0
	- Jinja2>=2.10.3
	NOTE: The requirements.txt file is located in tf-m\bl2\ext\mcuboot\scripts. The Python modules 
	may be installed using command: ``pip install -r requirements.txt``

MCUboot settings
----------------
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| Parameter       					| Description                                             					 | Default value 																		|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| DEFAULT_MCUBOOT_SECURITY_COUNTERS | Use default security TF-M project counter configuration          			 | ON																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+ 
| DEFAULT_MCUBOOT_FLASH_MAP         | Use default TF-M project flash map   										 | ON																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_IMAGE_NUMBER     			| Combine S and NS into 1 image, or sign each seperately         			 | 2																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_EXECUTION_SLOT   			| Slot from which to execute the image, used for XIP mode    				 | 1																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_LOG_LEVEL            		| Level of logging [OFF, ERROR, WARNING, INFO, DEBUG]   					 | INFO																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_HW_KEY    				| Embed the entire public key in the image metadata instead of the hash only | ON																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_UPGRADE_STRATEGY          | Upgrade strategy for images                 								 | OVERWRITE_ONLY																		|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_DIRECT_XIP_REVERT         | Enable the revert mechanism in direct-xip mode                 			 | ON																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_MEASURED_BOOT             | Add boot measurement values to boot status                 				 | ON																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_HW_ROLLBACK_PROT          | Enable security counter validation against non-volatile HW counters 		 | ON																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_ENC_IMAGES             	| Enable encrypted image upgrade support                 					 | OFF																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_ENCRYPT_RSA            	| Use RSA for encrypted image upgrade support                 				 | OFF																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_FIH_PROFILE             	| Fault injection hardening profile [OFF, LOW, MEDIUM, HIGH]                 | OFF																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_SIGNATURE_TYPE            | Algorithm to use for signature validation                 				 | RSA																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_SIGNATURE_KEY_LEN         | Key length to use for signature validation                 				 | 3072																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_KEY_S             		| Path to key with which to sign secure binary                 				 | tf-m/bl2/ext/mcuboot/root-${MCUBOOT_SIGNATURE_TYPE}-${MCUBOOT_SIGNATURE_KEY_LEN}.pem	|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_KEY_NS             		| Path to key with which to sign non-secure binary                 			 | tf-m/bl2/ext/mcuboot/root-${MCUBOOT_SIGNATURE_TYPE}-${MCUBOOT_SIGNATURE_KEY_LEN}.pem	|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_IMAGE_VERSION_S           | Version number of S image                 								 | ${TFM_VERSION}																		|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_IMAGE_VERSION_NS          | Version number of NS image                 								 | 0.0.0																				|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+ 
| MCUBOOT_SECURITY_COUNTER_S        | Security counter for S image. auto sets it to IMAGE_VERSION_S              | auto																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+ 
| MCUBOOT_SECURITY_COUNTER_NS		| Security counter for NS image. auto sets it to IMAGE_VERSION_NS            | auto																					|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_S_IMAGE_MIN_VER           | Minimum version of secure image required by the non-secure image           | 0.0.0+0																				|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+
| MCUBOOT_NS_IMAGE_MIN_VER          | Minimum version of non-secure image required by the secure image           | 0.0.0+0																				|
+-----------------------------------+----------------------------------------------------------------------------+--------------------------------------------------------------------------------------+

Image signing
-------------
Secure & non-secure image signing with configured keys is done automatically during build using scripts located in tf-m\bl2\ext\mcuboot\scripts. 
Signing operation is done separately for the secure and non-secure image using wrapper.py script with the following required attributes:
	--version 
    --layout
    --key
    --public-key-format
    --align
    --pad
    --pad-header
    --header-size 
      INFILE 
	  OUTFILE
	  
When MCUBOOT_IMAGE_NUMBER=1 (defaultlty implemented for MCUXpresso IDE), it is needed to use assemble.py script for merging the secure image and non-secure image together.