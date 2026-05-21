Hardware requirements
=====================
- Micro USB cable
- FRDMMCXA577 board
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
The log below shows the output of the TFM regression tests in the terminal window:
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Booting TF-M v2.1.1
[INF] Beginning TF-M provisioning
[WRN] TFM_DUMMY_PROVISIONING is not suitable for production! This device is NOT SECURE
[WRN] This device was provisioned with dummy keys. This device is NOT SECURE
[Sec Thread] Secure image initializing!
TF-M Float ABI: Hard
Lazy stacking enabled
Creating an empty ITS flash layout.

#### Execute test suites for the Secure area ####
Running Test Suite PSA internal trusted storage S interface tests (TFM_S_ITS_TEST_1XXX)...
> Executing 'TFM_S_ITS_TEST_1001'
  Description: 'Set interface'
  TEST: TFM_S_ITS_TEST_1001 - PASSED!
> Executing 'TFM_S_ITS_TEST_1002'
  Description: 'Set interface with create flags and get latest set flag'
  TEST: TFM_S_ITS_TEST_1002 - PASSED!
> Executing 'TFM_S_ITS_TEST_1003'
  Description: 'Set interface with NULL data pointer'
  TEST: TFM_S_ITS_TEST_1003 - PASSED!
> Executing 'TFM_S_ITS_TEST_1004'
  Description: 'Set interface with write once UID'
  TEST: TFM_S_ITS_TEST_1004 - PASSED!
> Executing 'TFM_S_ITS_TEST_1005'
  Description: 'Get interface with valid data'
  TEST: TFM_S_ITS_TEST_1005 - PASSED!
> Executing 'TFM_S_ITS_TEST_1006'
  Description: 'Get interface with zero data length'
  TEST: TFM_S_ITS_TEST_1006 - PASSED!
> Executing 'TFM_S_ITS_TEST_1007'
  Description: 'Get interface with invalid UIDs'
  TEST: TFM_S_ITS_TEST_1007 - PASSED!
> Executing 'TFM_S_ITS_TEST_1008'
  Description: 'Get interface with data lengths and offsets greater than UID length'
  TEST: TFM_S_ITS_TEST_1008 - PASSED!
> Executing 'TFM_S_ITS_TEST_1009'
  Description: 'Get interface with NULL data pointer'
  TEST: TFM_S_ITS_TEST_1009 - PASSED!
> Executing 'TFM_S_ITS_TEST_1010'
  Description: 'Get info interface with write once UID'
  TEST: TFM_S_ITS_TEST_1010 - PASSED!
> Executing 'TFM_S_ITS_TEST_1011'
  Description: 'Get info interface with valid UID'
  TEST: TFM_S_ITS_TEST_1011 - PASSED!
> Executing 'TFM_S_ITS_TEST_1012'
  Description: 'Get info interface with invalid UIDs'
  TEST: TFM_S_ITS_TEST_1012 - PASSED!
> Executing 'TFM_S_ITS_TEST_1013'
  Description: 'Remove interface with valid UID'
  TEST: TFM_S_ITS_TEST_1013 - PASSED!
> Executing 'TFM_S_ITS_TEST_1014'
  Description: 'Remove interface with write once UID'
  TEST: TFM_S_ITS_TEST_1014 - PASSED!
> Executing 'TFM_S_ITS_TEST_1015'
  Description: 'Remove interface with invalid UID'
  TEST: TFM_S_ITS_TEST_1015 - PASSED!
> Executing 'TFM_S_ITS_TEST_1016'
  Description: 'Block compaction after remove'
  TEST: TFM_S_ITS_TEST_1016 - PASSED!
> Executing 'TFM_S_ITS_TEST_1017'
  Description: 'Multiple partial gets'
  TEST: TFM_S_ITS_TEST_1017 - PASSED!
> Executing 'TFM_S_ITS_TEST_1018'
  Description: 'Multiple sets to same UID from same thread'
  TEST: TFM_S_ITS_TEST_1018 - PASSED!
> Executing 'TFM_S_ITS_TEST_1019'
  Description: 'Set, get and remove interface with different asset sizes'
  TEST: TFM_S_ITS_TEST_1019 - PASSED!
> Executing 'TFM_S_ITS_TEST_1020'
  Description: 'Set with asset size that exceeds the maximum'
  TEST: TFM_S_ITS_TEST_1020 - PASSED!
> Executing 'TFM_S_ITS_TEST_1021'
  Description: 'Get interface with data_len over 0 and NULL data length pointer'
  TEST: TFM_S_ITS_TEST_1021 - PASSED!
> Executing 'TFM_S_ITS_TEST_1023'
  Description: 'Attempt to get a UID set by a different partition'
  TEST: TFM_S_ITS_TEST_1023 - PASSED!
TESTSUITE PASSED!
Running Test Suite ITS reliability tests (TFM_ITS_TEST_2XXX)...
> Executing 'TFM_S_ITS_TEST_2001'
  Description: 'repetitive sets and gets in/from an asset'
  > Iteration 1 of 15
  > Iteration 2 of 15
  > Iteration 3 of 15
  > Iteration 4 of 15
  > Iteration 5 of 15
  > Iteration 6 of 15
  > Iteration 7 of 15
  > Iteration 8 of 15
  > Iteration 9 of 15
  > Iteration 10 of 15
  > Iteration 11 of 15
  > Iteration 12 of 15
  > Iteration 13 of 15
  > Iteration 14 of 15
  > Iteration 15 of 15
  TEST: TFM_S_ITS_TEST_2001 - PASSED!
> Executing 'TFM_S_ITS_TEST_2002'
  Description: 'repetitive sets, gets and removes'
  > Iteration 1 of 15
  > Iteration 2 of 15
  > Iteration 3 of 15
  > Iteration 4 of 15
  > Iteration 5 of 15
  > Iteration 6 of 15
  > Iteration 7 of 15
  > Iteration 8 of 15
  > Iteration 9 of 15
  > Iteration 10 of 15
  > Iteration 11 of 15
  > Iteration 12 of 15
  > Iteration 13 of 15
  > Iteration 14 of 15
  > Iteration 15 of 15
  TEST: TFM_S_ITS_TEST_2002 - PASSED!
TESTSUITE PASSED!
Running Test Suite Crypto secure interface tests (TFM_S_CRYPTO_TEST_1XXX)...
> Executing 'TFM_S_CRYPTO_TEST_1001'
  Description: 'Secure Key management interface'
  TEST: TFM_S_CRYPTO_TEST_1001 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1007'
  Description: 'Secure Symmetric encryption invalid cipher'
  TEST: TFM_S_CRYPTO_TEST_1007 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1008'
  Description: 'Secure Symmetric encryption invalid cipher (AES-152)'
  TEST: TFM_S_CRYPTO_TEST_1008 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1010'
  Description: 'Secure Unsupported Hash (SHA-1) interface'
  TEST: TFM_S_CRYPTO_TEST_1010 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1012'
  Description: 'Secure Hash (SHA-256) interface'
  TEST: TFM_S_CRYPTO_TEST_1012 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1019'
  Description: 'Secure Unsupported HMAC (SHA-1) interface'
  TEST: TFM_S_CRYPTO_TEST_1019 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1020'
  Description: 'Secure HMAC (SHA-256) interface'
  TEST: TFM_S_CRYPTO_TEST_1020 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1030'
  Description: 'Secure AEAD (AES-128-CCM) interface'
  TEST: TFM_S_CRYPTO_TEST_1030 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1032'
  Description: 'Secure key policy interface'
  TEST: TFM_S_CRYPTO_TEST_1032 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1033'
  Description: 'Secure key policy check permissions'
Two cipher modes are required. Skipping...
  TEST: TFM_S_CRYPTO_TEST_1033 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1035'
  Description: 'Key access control'
  TEST: TFM_S_CRYPTO_TEST_1035 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1036'
  Description: 'Secure AEAD interface with truncated auth tag (AES-128-CCM-8)'
  TEST: TFM_S_CRYPTO_TEST_1036 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1037'
  Description: 'Secure TLS 1.2 PRF key derivation'
  TEST: TFM_S_CRYPTO_TEST_1037 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1038'
  Description: 'Secure TLS-1.2 PSK-to-MasterSecret key derivation'
  TEST: TFM_S_CRYPTO_TEST_1038 - PASSED!
TESTSUITE PASSED!
Running Test Suite Symmetric key algorithm based Initial Attestation Service secure interface tests (TFM_S_ATTEST_TEST_2XXX)...
> Executing 'TFM_S_ATTEST_TEST_2001'
  Description: 'Symmetric key algorithm based Initial Attestation test'
  TEST: TFM_S_ATTEST_TEST_2001 - PASSED!
TESTSUITE PASSED!
Running Test Suite Platform Service Secure interface tests(TFM_S_PLATFORM_TEST_1XXX)...
> Executing 'TFM_S_PLATFORM_TEST_1001'
  Description: 'Minimal platform service test'
  TEST: TFM_S_PLATFORM_TEST_1001 - PASSED!
TESTSUITE PASSED!

*** Secure test suites summary ***
Test suite 'PSA internal trusted storage S interface tests (TFM_S_ITS_TEST_1XXX)' has PASSED
Test suite 'ITS reliability tests (TFM_ITS_TEST_2XXX)' has PASSED
Test suite 'Crypto secure interface tests (TFM_S_CRYPTO_TEST_1XXX)' has PASSED
Test suite 'Symmetric key algorithm based Initial Attestation Service secure interface tests (TFM_S_ATTEST_TEST_2XXX)' has PASSED
Test suite 'Platform Service Secure interface tests(TFM_S_PLATFORM_TEST_1XXX)' has PASSED

*** End of Secure test suites ***
Non-Secure system starting...

#### Execute test suites for the Non-secure area ####
Running Test Suite PSA internal trusted storage NS interface tests (TFM_NS_ITS_TEST_1XXX)...
> Executing 'TFM_NS_ITS_TEST_1001'
  Description: 'Set interface'
  TEST: TFM_NS_ITS_TEST_1001 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1002'
  Description: 'Set interface with create flags and get latest set flag'
  TEST: TFM_NS_ITS_TEST_1002 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1003'
  Description: 'Set interface with NULL data pointer'
  TEST: TFM_NS_ITS_TEST_1003 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1004'
  Description: 'Set interface with write once UID'
  TEST: TFM_NS_ITS_TEST_1004 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1005'
  Description: 'Get interface with valid data'
  TEST: TFM_NS_ITS_TEST_1005 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1006'
  Description: 'Get interface with zero data length'
  TEST: TFM_NS_ITS_TEST_1006 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1007'
  Description: 'Get interface with invalid UIDs'
  TEST: TFM_NS_ITS_TEST_1007 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1008'
  Description: 'Get interface with invalid data lengths and offsets'
  TEST: TFM_NS_ITS_TEST_1008 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1009'
  Description: 'Get interface with NULL data pointer'
  TEST: TFM_NS_ITS_TEST_1009 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1010'
  Description: 'Get info interface with write once UID'
  TEST: TFM_NS_ITS_TEST_1010 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1011'
  Description: 'Get info interface with valid UID'
  TEST: TFM_NS_ITS_TEST_1011 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1012'
  Description: 'Get info interface with invalid UIDs'
  TEST: TFM_NS_ITS_TEST_1012 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1013'
  Description: 'Remove interface with valid UID'
  TEST: TFM_NS_ITS_TEST_1013 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1014'
  Description: 'Remove interface with write once UID'
  TEST: TFM_NS_ITS_TEST_1014 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1015'
  Description: 'Remove interface with invalid UID'
  TEST: TFM_NS_ITS_TEST_1015 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1016'
  Description: 'Block compaction after remove'
  TEST: TFM_NS_ITS_TEST_1016 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1017'
  Description: 'Multiple partial gets'
  TEST: TFM_NS_ITS_TEST_1017 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1018'
  Description: 'Multiple sets to same UID from same thread'
  TEST: TFM_NS_ITS_TEST_1018 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1019'
  Description: 'Set, get and remove interface with different asset sizes'
  TEST: TFM_NS_ITS_TEST_1019 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1020'
  Description: 'Set with asset size that exceeds the maximum'
  TEST: TFM_NS_ITS_TEST_1020 - PASSED!
> Executing 'TFM_NS_ITS_TEST_1021'
  Description: 'Get interface with data_len over 0 and NULL data length pointer'
  TEST: TFM_NS_ITS_TEST_1021 - PASSED!
> Executing 'TFM_NS_ITS_TEST_001'
  Description: 'Set interface with NULL data pointer and length over 0'
  TEST: TFM_NS_ITS_TEST_001 - PASSED!
> Executing 'TFM_NS_ITS_TEST_002'
  Description: 'Get info interface with NULL p_info'
  TEST: TFM_NS_ITS_TEST_002 - PASSED!
> Executing 'TFM_NS_ITS_TEST_003'
  Description: 'Get interface with NULL data pointer and data_size over 0'
  TEST: TFM_NS_ITS_TEST_003 - PASSED!
TESTSUITE PASSED!
Running Test Suite Crypto non-secure interface test (TFM_NS_CRYPTO_TEST_1XXX)...
> Executing 'TFM_NS_CRYPTO_TEST_1001'
  Description: 'Non Secure Key management interface'
  TEST: TFM_NS_CRYPTO_TEST_1001 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1007'
  Description: 'Non Secure Symmetric encryption invalid cipher'
  TEST: TFM_NS_CRYPTO_TEST_1007 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1008'
  Description: 'Non Secure Symmetric encryption invalid cipher (AES-152)'
  TEST: TFM_NS_CRYPTO_TEST_1008 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1010'
  Description: 'Non Secure Unsupported Hash (SHA-1) interface'
  TEST: TFM_NS_CRYPTO_TEST_1010 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1012'
  Description: 'Non Secure Hash (SHA-256) interface'
  TEST: TFM_NS_CRYPTO_TEST_1012 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1019'
  Description: 'Non Secure Unsupported HMAC (SHA-1) interface'
  TEST: TFM_NS_CRYPTO_TEST_1019 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1020'
  Description: 'Non Secure HMAC (SHA-256) interface'
  TEST: TFM_NS_CRYPTO_TEST_1020 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1030'
  Description: 'Non Secure AEAD (AES-128-CCM) interface'
  TEST: TFM_NS_CRYPTO_TEST_1030 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1032'
  Description: 'Non Secure key policy interface'
  TEST: TFM_NS_CRYPTO_TEST_1032 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1033'
  Description: 'Non Secure key policy check permissions'
Two cipher modes are required. Skipping...
  TEST: TFM_NS_CRYPTO_TEST_1033 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1035'
  Description: 'Non Secure AEAD interface with truncated auth tag (AES-128-CCM-8)'
  TEST: TFM_NS_CRYPTO_TEST_1035 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1036'
  Description: 'Non Secure TLS 1.2 PRF key derivation'
  TEST: TFM_NS_CRYPTO_TEST_1036 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1037'
  Description: 'Non Secure TLS-1.2 PSK-to-MasterSecret key derivation'
  TEST: TFM_NS_CRYPTO_TEST_1037 - PASSED!
TESTSUITE PASSED!
Running Test Suite Platform Service Non-Secure interface tests(TFM_NS_PLATFORM_TEST_1XXX)...
> Executing 'TFM_NS_PLATFORM_TEST_1001'
  Description: 'Minimal platform service test'
  TEST: TFM_NS_PLATFORM_TEST_1001 - PASSED!
TESTSUITE PASSED!
Running Test Suite Symmetric key algorithm based Initial Attestation Service non-secure interface tests (TFM_NS_ATTEST_TEST_2XXX)...
> Executing 'TFM_NS_ATTEST_TEST_2001'
  Description: 'Symmetric key algorithm based Initial Attestation test'
  TEST: TFM_NS_ATTEST_TEST_2001 - PASSED!
TESTSUITE PASSED!

*** Non-secure test suites summary ***
Test suite 'PSA internal trusted storage NS interface tests (TFM_NS_ITS_TEST_1XXX)' has PASSED
Test suite 'Crypto non-secure interface test (TFM_NS_CRYPTO_TEST_1XXX)' has PASSED
Test suite 'Platform Service Non-Secure interface tests(TFM_NS_PLATFORM_TEST_1XXX)' has PASSED
Test suite 'Symmetric key algorithm based Initial Attestation Service non-secure interface tests (TFM_NS_ATTEST_TEST_2XXX)' has PASSED

*** End of Non-secure test suites ***
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

