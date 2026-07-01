Hardware requirements
=====================
- Micro USB cable
- frdmimxrt700 board
- Personal Computer

Board settings
============
Switch the JP45 jumper to the 2-3 configuration and the JP46 jumper to the 2-3 configuration to enable XSPI1 memory access.
Switch the SW10 boot mode switch to the 11 XSPI1 flash configuration.

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

FRDM-RT700 - TF-M Integration Notes
==========================================

1- Internal Trusted Storage (ITS) Support
------------------------------------------

The current TF-M implementation for the FRDM-RT700 platform supports Internal Trusted Storage (ITS) using the external flash (XSP1). While this enables persistent storage capabilities, it is important to note the following limitations and considerations:

* Unprotected External Flash: The XSP1 flash used for ITS is external and does not provide encryption or hardware-based protection by default. This means that sensitive data stored in this region is potentially vulnerable to unauthorized access.

* User Responsibility for Security: It is the user's responsibility to ensure appropriate protection mechanisms are in place for the XSP1 flash region. This may include implementing custom encryption, access control, or secure boot mechanisms.

* Alternative ITS in RAM: For applications requiring higher security guarantees, users may opt to configure ITS to use RAM-based storage. While this approach sacrifices persistence across power cycles, it offers better protection for sensitive data in scenarios where external flash cannot be adequately secured.

Future Enhancements
------------------------------------------

* Support for IPED-based protection of the XSP1 flash is planned in future TF-M releases for FRDM-RT700. This will enable hardware-enforced isolation and encryption, significantly improving the security of ITS data stored in external flash.
