# secure_faults_s

SECURE VULNERABILITY NOTE:
There is a vulnerability that can affect TrustZone applications that also use floating point unit (CVE-2021-35465). 
The TrustZone examples delivered here do not use floating point, and so they aren't affected. However, 
if TrustZone and FPU are used together, the used compiler should be checked for inbuilt workaround. 
If workaround is not available yet, then the actions described in CVE-2021-35465 should be applied manually.

Actual compilers status for this vulnerability:
MCUX IDE v11.5.0 – fix not available yet
GNU Arm Embedded Toolchain 10.3-2021.07 - fix not available yet
EWARM v9.20.1 – already fixed
ARM MDK v5.35 – check for new Arm compiler v6.17

## Overview
The Secure Faults demo application demonstrates handling of different secure faults. This application is based 
on application Hello World. In addition, user can invoke different secure faults by setting testCaseNumber variable (see source code).
The following faults can be invoked:

TEST 0: No any secure fault
TEST 1: Invalid transition from secure to normal world
TEST 2: Invalid entry point from normal to secure world
TEST 3: Invalid data access from normal world, example 1
TEST 4: Invalid input parameters in entry function
TEST 5: Invalid data access from normal world, example 2

## Supported Boards
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/trustzone_examples/secure_faults/secure_faults_s/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/trustzone_examples/secure_faults/secure_faults_s/example_board_readme.md)
- [FRDM-MCXN236](../../_boards/frdmmcxn236/trustzone_examples/secure_faults/secure_faults_s/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/trustzone_examples/secure_faults/secure_faults_s/example_board_readme.md)
- [LPCXpresso55S06](../../_boards/lpcxpresso55s06/trustzone_examples/secure_faults/secure_faults_s/example_board_readme.md)
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/trustzone_examples/secure_faults/secure_faults_s/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/trustzone_examples/secure_faults/secure_faults_s/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/trustzone_examples/secure_faults/secure_faults_s/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/trustzone_examples/secure_faults/secure_faults_s/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/trustzone_examples/secure_faults/secure_faults_s/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/trustzone_examples/secure_faults/secure_faults_s/example_board_readme.md)
