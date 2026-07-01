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
4.  Launch the debugger in your IDE to begin running the demo.

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
Creating an empty PS flash layout.
[INF][PS] Encryption alg: 0x5500100

#### Execute test suites for the Secure area ####
Running Test Suite IPC secure interface test (TFM_S_IPC_TEST_1XXX)...
> Executing 'TFM_S_IPC_TEST_1001' 
  Description: 'Get PSA framework version'
  TEST: TFM_S_IPC_TEST_1001 - PASSED!
> Executing 'TFM_S_IPC_TEST_1002' 
  Description: 'Get version of an RoT Service'
  TEST: TFM_S_IPC_TEST_1002 - PASSED!
> Executing 'TFM_S_IPC_TEST_1004' 
  Description: 'Request connection-based RoT Service'
  TEST: TFM_S_IPC_TEST_1004 - PASSED!
> Executing 'TFM_S_IPC_TEST_1006' 
  Description: 'Call PSA RoT access APP RoT memory test service'
Connect success!
Call success!
  TEST: TFM_S_IPC_TEST_1006 - PASSED!
> Executing 'TFM_S_IPC_TEST_1012' 
  Description: 'Request stateless service'
  TEST: TFM_S_IPC_TEST_1012 - PASSED!
TESTSUITE PASSED!
Running Test Suite PSA protected storage S interface tests (TFM_S_PS_TEST_1XXX)...
> Executing 'TFM_S_PS_TEST_1001' 
  Description: 'Set interface'
  TEST: TFM_S_PS_TEST_1001 - PASSED!
> Executing 'TFM_S_PS_TEST_1002' 
  Description: 'Set interface with create flags'
  TEST: TFM_S_PS_TEST_1002 - PASSED!
> Executing 'TFM_S_PS_TEST_1003' 
  Description: 'Set interface with NULL data pointer'
  TEST: TFM_S_PS_TEST_1003 - PASSED!
> Executing 'TFM_S_PS_TEST_1005' 
  Description: 'Set interface with write once UID'
  TEST: TFM_S_PS_TEST_1005 - PASSED!
> Executing 'TFM_S_PS_TEST_1006' 
  Description: 'Get interface with valid data'
  TEST: TFM_S_PS_TEST_1006 - PASSED!
> Executing 'TFM_S_PS_TEST_1007' 
  Description: 'Get interface with zero data length'
  TEST: TFM_S_PS_TEST_1007 - PASSED!
> Executing 'TFM_S_PS_TEST_1008' 
  Description: 'Get interface with invalid UIDs'
  TEST: TFM_S_PS_TEST_1008 - PASSED!
> Executing 'TFM_S_PS_TEST_1009' 
  Description: 'Get interface with invalid data lengths and offsets'
  TEST: TFM_S_PS_TEST_1009 - PASSED!
> Executing 'TFM_S_PS_TEST_1010' 
  Description: 'Get interface with NULL data pointer'
  TEST: TFM_S_PS_TEST_1010 - PASSED!
> Executing 'TFM_S_PS_TEST_1011' 
  Description: 'Get info interface with write once UID'
  TEST: TFM_S_PS_TEST_1011 - PASSED!
> Executing 'TFM_S_PS_TEST_1012' 
  Description: 'Get info interface with valid UID'
  TEST: TFM_S_PS_TEST_1012 - PASSED!
> Executing 'TFM_S_PS_TEST_1013' 
  Description: 'Get info interface with invalid UIDs'
  TEST: TFM_S_PS_TEST_1013 - PASSED!
> Executing 'TFM_S_PS_TEST_1015' 
  Description: 'Remove interface with valid UID'
  TEST: TFM_S_PS_TEST_1015 - PASSED!
> Executing 'TFM_S_PS_TEST_1016' 
  Description: 'Remove interface with write once UID'
  TEST: TFM_S_PS_TEST_1016 - PASSED!
> Executing 'TFM_S_PS_TEST_1017' 
  Description: 'Remove interface with invalid UID'
  TEST: TFM_S_PS_TEST_1017 - PASSED!
> Executing 'TFM_S_PS_TEST_1018' 
  Description: 'Block compaction after remove'
  TEST: TFM_S_PS_TEST_1018 - PASSED!
> Executing 'TFM_S_PS_TEST_1019' 
  Description: 'Multiple partial gets'
  TEST: TFM_S_PS_TEST_1019 - PASSED!
> Executing 'TFM_S_PS_TEST_1020' 
  Description: 'Multiple sets to same UID from same thread'
  TEST: TFM_S_PS_TEST_1020 - PASSED!
> Executing 'TFM_S_PS_TEST_1021' 
  Description: 'Get support interface'
  TEST: TFM_S_PS_TEST_1021 - PASSED!
> Executing 'TFM_S_PS_TEST_1022' 
  Description: 'Set, get and remove interface with different asset sizes'
  TEST: TFM_S_PS_TEST_1022 - PASSED!
TESTSUITE PASSED!
Running Test Suite PS reliability tests (TFM_PS_TEST_3XXX)...
> Executing 'TFM_S_PS_TEST_2001' 
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
  TEST: TFM_S_PS_TEST_2001 - PASSED!
> Executing 'TFM_S_PS_TEST_2002' 
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
  TEST: TFM_S_PS_TEST_2002 - PASSED!
TESTSUITE PASSED!
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
> Executing 'TFM_S_CRYPTO_TEST_1002' 
  Description: 'Secure Symmetric encryption (AES-128-CBC) interface'
  TEST: TFM_S_CRYPTO_TEST_1002 - PASSED!
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
> Executing 'TFM_S_CRYPTO_TEST_1014' 
  Description: 'Secure Hash (SHA-512) interface'
  TEST: TFM_S_CRYPTO_TEST_1014 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1019' 
  Description: 'Secure Unsupported HMAC (SHA-1) interface'
  TEST: TFM_S_CRYPTO_TEST_1019 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1020' 
  Description: 'Secure HMAC (SHA-256) interface'
  TEST: TFM_S_CRYPTO_TEST_1020 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1022' 
  Description: 'Secure HMAC (SHA-512) interface'
  TEST: TFM_S_CRYPTO_TEST_1022 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1030' 
  Description: 'Secure AEAD (AES-128-CCM) interface'
  TEST: TFM_S_CRYPTO_TEST_1030 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1031' 
  Description: 'Secure AEAD (AES-128-GCM) interface'
  TEST: TFM_S_CRYPTO_TEST_1031 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1032' 
  Description: 'Secure key policy interface'
  TEST: TFM_S_CRYPTO_TEST_1032 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1033' 
  Description: 'Secure key policy check permissions'
alg: 4404000, not_permit_alg: 0. Unable to find two Cipher algs. Skipping...
  TEST: TFM_S_CRYPTO_TEST_1033 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1034' 
  Description: 'Secure persistent key interface'
  TEST: TFM_S_CRYPTO_TEST_1034 - PASSED!
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
> Executing 'TFM_S_CRYPTO_TEST_1039' 
  Description: 'Secure HKDF key derivation'
  TEST: TFM_S_CRYPTO_TEST_1039 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1040' 
  Description: 'Secure ECDH key agreement'
  TEST: TFM_S_CRYPTO_TEST_1040 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1043' 
  Description: 'Secure Asymmetric encryption interface (RSA-OAEP)'
  TEST: TFM_S_CRYPTO_TEST_1043 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1044' 
  Description: 'Secure Asymmetric encryption interface (RSA-PKCS1V15)'
  TEST: TFM_S_CRYPTO_TEST_1044 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1046' 
  Description: 'Secure Symmetric encryption (AES-128-CBC-PKCS7) interface'
  TEST: TFM_S_CRYPTO_TEST_1046 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1047' 
  Description: 'Secure Symmetric encryption (AES-128-CBC-PKCS7) interface, shorter'
  TEST: TFM_S_CRYPTO_TEST_1047 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1048' 
  Description: 'Secure Symmetric encryption (AES-128-CBC-PKCS7) interface, longer'
  TEST: TFM_S_CRYPTO_TEST_1048 - PASSED!
> Executing 'TFM_S_CRYPTO_TEST_1055' 
  Description: 'Secure GCM authenticator'
REF(0)TAG: 0x60, 0x9b, 0x3d, 0x51, 0x91, 0x60, 0x8, 0x17, 0x82, 0xec, 0x63, 0x21, 0x3a, 0x4, 0xdc, 0x93, 
REF(1)TAG: 0xff, 0x66, 0xb8, 0x6, 0x7b, 0x76, 0x91, 0x8f, 0x76, 0x49, 0x2e, 0x56, 0xe2, 0xa2, 0x80, 0x62, 
  TEST: TFM_S_CRYPTO_TEST_1055 - PASSED!
TESTSUITE PASSED!
Running Test Suite Initial Attestation Service secure interface tests(TFM_S_ATTEST_TEST_1XXX)...
> Executing 'TFM_S_ATTEST_TEST_1004' 
  Description: 'ECDSA signature test of attest token'
  TEST: TFM_S_ATTEST_TEST_1004 - PASSED!
> Executing 'TFM_S_ATTEST_TEST_1005' 
  Description: 'Negative test cases for initial attestation service'
  TEST: TFM_S_ATTEST_TEST_1005 - PASSED!
TESTSUITE PASSED!
Running Test Suite Platform Service Secure interface tests(TFM_S_PLATFORM_TEST_1XXX)...
> Executing 'TFM_S_PLATFORM_TEST_1001' 
  Description: 'Minimal platform service test'
  TEST: TFM_S_PLATFORM_TEST_1001 - PASSED!
TESTSUITE PASSED!

*** Secure test suites summary ***
Test suite 'IPC secure interface test (TFM_S_IPC_TEST_1XXX)' has PASSED
Test suite 'PSA protected storage S interface tests (TFM_S_PS_TEST_1XXX)' has PASSED
Test suite 'PS reliability tests (TFM_PS_TEST_3XXX)' has PASSED
Test suite 'PSA internal trusted storage S interface tests (TFM_S_ITS_TEST_1XXX)' has PASSED
Test suite 'ITS reliability tests (TFM_ITS_TEST_2XXX)' has PASSED
Test suite 'Crypto secure interface tests (TFM_S_CRYPTO_TEST_1XXX)' has PASSED
Test suite 'Initial Attestation Service secure interface tests(TFM_S_ATTEST_TEST_1XXX)' has PASSED
Test suite 'Platform Service Secure interface tests(TFM_S_PLATFORM_TEST_1XXX)' has PASSED

*** End of Secure test suites ***
Non-Secure system starting...

#### Execute test suites for the Non-secure area ####
Running Test Suite IPC non-secure interface test (TFM_NS_IPC_TEST_1XXX)...
> Executing 'TFM_NS_IPC_TEST_1001' 
  Description: 'Get PSA framework version'
  TEST: TFM_NS_IPC_TEST_1001 - PASSED!
> Executing 'TFM_NS_IPC_TEST_1002' 
  Description: 'Get version of an RoT Service'
  TEST: TFM_NS_IPC_TEST_1002 - PASSED!
> Executing 'TFM_NS_IPC_TEST_1003' 
  Description: 'Connect to an RoT Service'
Connect success!
  TEST: TFM_NS_IPC_TEST_1003 - PASSED!
> Executing 'TFM_NS_IPC_TEST_1004' 
  Description: 'Request connection-based RoT Service'
  TEST: TFM_NS_IPC_TEST_1004 - PASSED!
> Executing 'TFM_NS_IPC_TEST_1010' 
  Description: 'Test psa_call with the status of PSA_ERROR_PROGRAMMER_ERROR'
Connect success!
The first time call success!
The second time call success!
  TEST: TFM_NS_IPC_TEST_1010 - PASSED!
> Executing 'TFM_NS_IPC_TEST_1012' 
  Description: 'Request stateless service'
  TEST: TFM_NS_IPC_TEST_1012 - PASSED!
> Executing 'TFM_NS_IPC_TEST_1016' 
  Description: 'Testing Client-Id Translation'
  TEST: TFM_NS_IPC_TEST_1016 - PASSED!
TESTSUITE PASSED!
Running Test Suite PSA protected storage NS interface tests (TFM_NS_PS_TEST_1XXX)...
> Executing 'TFM_NS_PS_TEST_1001' 
  Description: 'Set interface'
  TEST: TFM_NS_PS_TEST_1001 - PASSED!
> Executing 'TFM_NS_PS_TEST_1002' 
  Description: 'Set interface with create flags'
  TEST: TFM_NS_PS_TEST_1002 - PASSED!
> Executing 'TFM_NS_PS_TEST_1003' 
  Description: 'Set interface with NULL data pointer'
  TEST: TFM_NS_PS_TEST_1003 - PASSED!
> Executing 'TFM_NS_PS_TEST_1004' 
  Description: 'Set interface with write once UID'
  TEST: TFM_NS_PS_TEST_1004 - PASSED!
> Executing 'TFM_NS_PS_TEST_1005' 
  Description: 'Get interface with valid data'
  TEST: TFM_NS_PS_TEST_1005 - PASSED!
> Executing 'TFM_NS_PS_TEST_1006' 
  Description: 'Get interface with zero data length'
  TEST: TFM_NS_PS_TEST_1006 - PASSED!
> Executing 'TFM_NS_PS_TEST_1007' 
  Description: 'Get interface with invalid UIDs'
  TEST: TFM_NS_PS_TEST_1007 - PASSED!
> Executing 'TFM_NS_PS_TEST_1008' 
  Description: 'Get interface with invalid data lengths and offsets'
  TEST: TFM_NS_PS_TEST_1008 - PASSED!
> Executing 'TFM_NS_PS_TEST_1009' 
  Description: 'Get interface with NULL data pointer'
  TEST: TFM_NS_PS_TEST_1009 - PASSED!
> Executing 'TFM_NS_PS_TEST_1010' 
  Description: 'Get info interface with write once UID'
  TEST: TFM_NS_PS_TEST_1010 - PASSED!
> Executing 'TFM_NS_PS_TEST_1011' 
  Description: 'Get info interface with valid UID'
  TEST: TFM_NS_PS_TEST_1011 - PASSED!
> Executing 'TFM_NS_PS_TEST_1012' 
  Description: 'Get info interface with invalid UIDs'
  TEST: TFM_NS_PS_TEST_1012 - PASSED!
> Executing 'TFM_NS_PS_TEST_1013' 
  Description: 'Remove interface with valid UID'
  TEST: TFM_NS_PS_TEST_1013 - PASSED!
> Executing 'TFM_NS_PS_TEST_1014' 
  Description: 'Remove interface with write once UID'
  TEST: TFM_NS_PS_TEST_1014 - PASSED!
> Executing 'TFM_NS_PS_TEST_1015' 
  Description: 'Remove interface with invalid UID'
  TEST: TFM_NS_PS_TEST_1015 - PASSED!
> Executing 'TFM_NS_PS_TEST_1021' 
  Description: 'Block compaction after remove'
  TEST: TFM_NS_PS_TEST_1021 - PASSED!
> Executing 'TFM_NS_PS_TEST_1022' 
  Description: 'Multiple partial gets'
  TEST: TFM_NS_PS_TEST_1022 - PASSED!
> Executing 'TFM_NS_PS_TEST_1023' 
  Description: 'Multiple sets to same UID from same thread'
  TEST: TFM_NS_PS_TEST_1023 - PASSED!
> Executing 'TFM_NS_PS_TEST_1024' 
  Description: 'Get support interface'
  TEST: TFM_NS_PS_TEST_1024 - PASSED!
> Executing 'TFM_NS_PS_TEST_1025' 
  Description: 'Set, get and remove interface with different asset sizes'
  TEST: TFM_NS_PS_TEST_1025 - PASSED!
TESTSUITE PASSED!
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
> Executing 'TFM_NS_CRYPTO_TEST_1002' 
  Description: 'Non Secure Symmetric encryption (AES-128-CBC) interface'
  TEST: TFM_NS_CRYPTO_TEST_1002 - PASSED!
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
> Executing 'TFM_NS_CRYPTO_TEST_1014' 
  Description: 'Non Secure Hash (SHA-512) interface'
  TEST: TFM_NS_CRYPTO_TEST_1014 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1019' 
  Description: 'Non Secure Unsupported HMAC (SHA-1) interface'
  TEST: TFM_NS_CRYPTO_TEST_1019 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1020' 
  Description: 'Non Secure HMAC (SHA-256) interface'
  TEST: TFM_NS_CRYPTO_TEST_1020 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1022' 
  Description: 'Non Secure HMAC (SHA-512) interface'
  TEST: TFM_NS_CRYPTO_TEST_1022 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1030' 
  Description: 'Non Secure AEAD (AES-128-CCM) interface'
  TEST: TFM_NS_CRYPTO_TEST_1030 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1031' 
  Description: 'Non Secure AEAD (AES-128-GCM) interface'
  TEST: TFM_NS_CRYPTO_TEST_1031 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1032' 
  Description: 'Non Secure key policy interface'
  TEST: TFM_NS_CRYPTO_TEST_1032 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1033' 
  Description: 'Non Secure key policy check permissions'
alg: 4404000, not_permit_alg: 0. Unable to find two Cipher algs. Skipping...
  TEST: TFM_NS_CRYPTO_TEST_1033 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1034' 
  Description: 'Non Secure persistent key interface'
  TEST: TFM_NS_CRYPTO_TEST_1034 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1035' 
  Description: 'Non Secure AEAD interface with truncated auth tag (AES-128-CCM-8)'
  TEST: TFM_NS_CRYPTO_TEST_1035 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1036' 
  Description: 'Non Secure TLS 1.2 PRF key derivation'
  TEST: TFM_NS_CRYPTO_TEST_1036 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1037' 
  Description: 'Non Secure TLS-1.2 PSK-to-MasterSecret key derivation'
  TEST: TFM_NS_CRYPTO_TEST_1037 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1038' 
  Description: 'Non Secure HKDF key derivation'
  TEST: TFM_NS_CRYPTO_TEST_1038 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1039' 
  Description: 'Non Secure ECDH key agreement'
  TEST: TFM_NS_CRYPTO_TEST_1039 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1042' 
  Description: 'Non Secure Asymmetric encryption interface (RSA-OAEP)'
  TEST: TFM_NS_CRYPTO_TEST_1042 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1043' 
  Description: 'Non Secure Asymmetric encryption interface (RSA-PKCS1V15)'
  TEST: TFM_NS_CRYPTO_TEST_1043 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1045' 
  Description: 'Non Secure Symmetric encryption (AES-128-CBC-PKCS7) interface'
  TEST: TFM_NS_CRYPTO_TEST_1045 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1046' 
  Description: 'Non Secure Symmetric encryption (AES-128-CBC-PKCS7) interface, shorter'
  TEST: TFM_NS_CRYPTO_TEST_1046 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1047' 
  Description: 'Non Secure Symmetric encryption (AES-128-CBC-PKCS7) interface, longer'
  TEST: TFM_NS_CRYPTO_TEST_1047 - PASSED!
> Executing 'TFM_NS_CRYPTO_TEST_1054' 
  Description: 'Non Secure GCM authenticator'
REF(0)TAG: 0x60, 0x9b, 0x3d, 0x51, 0x91, 0x60, 0x8, 0x17, 0x82, 0xec, 0x63, 0x21, 0x3a, 0x4, 0xdc, 0x93, 
REF(1)TAG: 0xff, 0x66, 0xb8, 0x6, 0x7b, 0x76, 0x91, 0x8f, 0x76, 0x49, 0x2e, 0x56, 0xe2, 0xa2, 0x80, 0x62, 
  TEST: TFM_NS_CRYPTO_TEST_1054 - PASSED!
TESTSUITE PASSED!
Running Test Suite Platform Service Non-Secure interface tests(TFM_NS_PLATFORM_TEST_1XXX)...
> Executing 'TFM_NS_PLATFORM_TEST_1001' 
  Description: 'Minimal platform service test'
  TEST: TFM_NS_PLATFORM_TEST_1001 - PASSED!
TESTSUITE PASSED!
Running Test Suite QCBOR regression test(TFM_NS_QCBOR_TEST_1XXX)...
> Executing 'TFM_NS_QCBOR_TEST_1001' 
  Description: 'Regression test of QCBOR library'
  TEST: TFM_NS_QCBOR_TEST_1001 - PASSED!
TESTSUITE PASSED!

*** Non-secure test suites summary ***
Test suite 'IPC non-secure interface test (TFM_NS_IPC_TEST_1XXX)' has PASSED
Test suite 'PSA protected storage NS interface tests (TFM_NS_PS_TEST_1XXX)' has PASSED
Test suite 'PSA internal trusted storage NS interface tests (TFM_NS_ITS_TEST_1XXX)' has PASSED
Test suite 'Crypto non-secure interface test (TFM_NS_CRYPTO_TEST_1XXX)' has PASSED
Test suite 'Platform Service Non-Secure interface tests(TFM_NS_PLATFORM_TEST_1XXX)' has PASSED
Test suite 'QCBOR regression test(TFM_NS_QCBOR_TEST_1XXX)' has PASSED

*** End of Non-secure test suites ***



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
