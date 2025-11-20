Hardware requirements
=====================
- Micro USB cable
- MIMXRT700-EVK board
- Personal Computer

Board settings
============

### XSPI1 Usage (Default Configuration)
To enable XSPI1 memory access, perform the following steps to enable XSPI1 memory access:
 - Set JP45 jumper to 2-3 configuration.
 - Set JP46 jumper to 2-3 configuration.
 - Configure SW10 boot mode switch to 11 (XSPI1 flash configuration).

### XSPI0 Usage:
 - Set JP45 jumper to 1-2 configuration.
 - Set JP46 jumper to 2-3 configuration.
 - Configure SW10 boot mode switch to 10 (XSPI0 flash configuration).
 
To use XSPI0 with TrustZone examples, you need to apply the following software changes:
1.  Update CMake Configuration:
    [trustzone_examples/<application_name>\<application_name>_s/cm33_core0/reconfig.cmake](reconfig.cmake)

    ```c
        DEMO_CODE_START_NS=672137216
    ```

2.  Update SAU Regions with following definations:
    [trustzone_examples/<application_name>\<application_name>_s/cm33_core0/tzm_config.c](tzm_config.c)

    ```c
        #define SAU_REGION_0_BASE 0x28100000U
        #define SAU_REGION_0_END 0x281FFFFFU
        #define SAU_REGION_2_BASE 0x280FFE00U
        #define SAU_REGION_2_END 0x280FFFFFU
    ```

3. Update Secure Linker Script with following:
  [trustzone_examples/linkscripts/MIMXRT798Sxxxx_cm33_core0_flash_s.icf](../../../linkscripts/MIMXRT798Sxxxx_cm33_core0_flash_s.icf)
    ```c
        define symbol m_text_start                     = 0x38004000;
        define symbol m_text_end                       = 0x380FFDFF;
        define symbol m_boot_flash_conf_start          = 0x38000000;
        define symbol m_veneer_table_start             = 0x280FFE00;
    ```
4. Update Non-Secure Linker Script with following:
  [trustzone_examples/linkscripts/MIMXRT798Sxxxx_cm33_core0_flash_ns.icf](../../../linkscripts/MIMXRT798Sxxxx_cm33_core0_flash_ns.icf)
    ```c
       define symbol m_text_start                     = 0x28100000;
       define symbol m_text_end                       = 0x281FFFFF;
       define symbol m_boot_flash_conf_start          = 0x28100000;
    ```
Note: XSPI0 usage is for IAR toolchain, for other tool chain, make similar changes in respective linker files.


Prepare the Demo
===============
1.  Connect a micro USB cable between the PC host and the debug port on the board
2.  Open a serial terminal with the following settings:
    - 115200 baud rate
    - 8 data bits
    - No parity
    - One stop bit
    - No flow control
3.  Use secure project to download the program to target board. Please refer to "TrustZone application debugging" below for details.
4.  Launch the debugger in your IDE to begin running the demo.

Running the demo
================
The log below shows the output of the hello world demo in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Hello from secure world!
Entering normal world.
Welcome in normal world!
This is a text printed from normal world!
Comparing two string as a callback to normal world
String 1: Test1
String 2: Test2
Both strings are not equal!                              
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

Project Structure
-----------------
The all TrustZone files related to TrustZone are located in trustzone virtual directory. The files are:

- tzm_config.c
- tzm_config.h
- veneer_table.c
- veneer_table.h

File tzm_config.c, tzm_config.h
------------------------------
This file is used by secure project only. It contains one function BOARD_InitTrustZone(), which configures complete TrustZone
environment. It includes SAU, MPU's, AHB secure controller and some TrustZone related registers from System Control Block.
This function is called from SystemInitHook() function, it means during system initialization.

File veneer_table.c, veneer_table.h
-----------------------------------
This file defines all secure functions (secure entry functions) exported to normal world. This file is located in secure
project only. While header file is used by both secure and non-secure projects. The secure entry functions usually contain
validation of all input parameters in order to avoid any data leak from secure world.

The files veneer_table.h and <application_name>_s_import_lib.o or <application_name>_s_CMSE_lib.o create the connection
between secure and non-secure projects. The library file is generated by linker during compilation of secure project and
it is linked to the non-secure project as any other library.

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

If want to download secure and non-secure binary file into flash, should apply the following rules:
Flash target:
    1. secure binary download into 0x08000000 address.
    2. non-secure binary download into 0x08100000 address.

Device header file and secure/non-secure access to the peripherals
------------------------------------------------------------------
Both secure and non-secure project uses identical device header file. The access to secure and non-secure aliases for all peripherals
is managed using compiler macro __ARM_FEATURE_CMSE.

For secure project using <PERIPH_BASE> means access through secure alias (address bit A28=1), 
using <PERIPH_BASE>_NS means access through non-secure alias(address bit A28=0)
For non-secure project using <PERIPH_BASE> means access through non-secure alias (address bit A28=0). 
The non-secure project doesn't have access to secure memory or peripherals regions so the secure access is not defined.
