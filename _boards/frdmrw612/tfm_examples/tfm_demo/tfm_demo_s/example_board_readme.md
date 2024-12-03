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
The log below shows the output of the TFM demo in the terminal window:
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
    
