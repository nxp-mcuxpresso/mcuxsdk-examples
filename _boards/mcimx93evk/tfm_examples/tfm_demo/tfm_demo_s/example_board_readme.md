Hardware requirements
=====================
- USB Type-C cable
- MCIMX93-EVK board
- J-Link Debug Probe
- 12V~20V power supply
- Personal Computer

Board settings
============

Note
====
- Please load the application from u-boot in Single boot mode. By default the example assumes secure binary will be loaded in TCM and non secure binary in OCRAM. If you are booting Linux on Cortex A with this example running on cortex M33, donot use ethos driver to avoid conflict as ethos driver used OCRAM.
- This example can be run from TCM memory also without using OCRAM. To do so, check Section "Compile TF-M to work only from TCM" below.

Prepare the Demo
===============
1.  Connect 12V~20V power supply and J-Link Debug Probe to the board, switch SW301 to power on the board.
2.  Connect a USB Type-C cable between the host PC and the J1401 USB port on the target board.
3.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
4.  Download the demo binaries to the target board.

	Reference "Running an application by U-Boot"
    https://mcuxpresso.nxp.com/mcuxsdk/latest/html/boards/i.MX/mcimx93evk/gettingStarted/topics/running_an_application_by_u-boot.html#running-an-application-by-u-boot
    Reference 'Getting Started with MCUXpresso SDK for MCIMX93-EVK.pdf' to download the demo binaries on board.

	Download the tfm_demo_s_cm33.bin to TCM, tfm_demo_ns_cm33.bin to OCRAM. Then boot the cortex M33 core.
	fatload mmc 1 0x80000000 tfm_demo_s_cm33.bin; cp.b 0x80000000 0x201e0000 0x20000
	fatload mmc 1 0x80020000 tfm_demo_ns_cm33.bin; cp.b 0x80020000 0x20480000 0x18000; dcache flush;
	bootaux 0x1ffe0000 0
	
Prints will appear on the serial terminal for Cortex M-33

Running the demo
================
The log below shows the output of the TFM demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Booting TF-M v2.1.1
[INF] Beginning TF-M provisioning
[WRN] TFM_DUMMY_PROVISIONING is not suitable for production! This device is NOT SECURE
[WRN] This device was provisioned with dummy keys. This device is NOT SECURE
[Sec Thread] Secure image initializing!
TF-M Float ABI: Hard
Lazy stacking enabled
Creating an empty ITS flash layout.
Creating an empty PS flash layout.
[INF][PS] Encryption alg: 0x5500100
Non-Secure system starting...
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

TF-M Features supported on MCIMX93EVK
----------------------------------------
Default secure binary on TCM ( max codesize 128K, data size 128K) , non secure binary to be loaded on ocram ( codesize 192k, data size 192k)

TF-M platform port assumes the trdc configuration has been done by cortex A core and secure privileged access has been granted to cortex M core domain. It would further program TRDC to divide resources between secure and non secure domain on cortex m core ( see platform/ext/target/target_cfg.c for more details)

Crypto partition is software based. Crypto accelerator is not supported on the platform.

By default storage is emulated on RAM ( ITS_ON_RAM, PS_ON_RAM). There is no flash support available as by default evk board comes without flash

SDK currently supports only medium profile builds.

Compile TF-M to work only from TCM
----------------------------------------
Total TCM available for code is only 128K on this platform.
To shift both secure and non secure binaries to TCM, the code sizes of both binaries should not be more than 128K.
To enable that, in file tf-m/platform/ext/target/nxp/mcimx93evk/partition/flash_layout.h comment the following line at line number 21 and recompile. 
#define NS_PARTITION_ON_OCRAM   1

In current configuration, if you disable the above option, given below is the Secure and Non secure code size chosen. You can change that based on your usecase in the flash-layout.h file.
#define IMAGE_S_CODE_SIZE   (96 * 1024)     /* Secure Binary code Max size */
#define IMAGE_NS_CODE_SIZE   (32 * 1024)     /* Non Secure Binary code Max size */

If such a change is done secure and non secure binaries should be loaded on required offsets in TCM. 
Example steps to do so from u-boot are given below:
fatload mmc 1 0x80000000 tfm_demo_s_cm33.bin
fatload mmc 1 0x80018000 tfm_demo_ns_cm33.bin
cp.b 0x80000000 0x201e0000 0x20000;
bootaux 0x1ffe0000 0


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

