# hello_world_s

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
The Hello World demo application provides a sanity check for the new SDK build environments and board bring up. This demo application also utilizes TrustZone, 
so it demonstrates following techniques for TrustZone applications development:
1. Application separation between secure and non-secure part
2. TrustZone environment configuration
3. Exporting secure function to non-secure world
4. Calling non-secure function from secure world
4. Creating veneer table
5. Configuring IAR, MDK, GCC and MCUX environments for TrustZone based projects

## Supported Boards
- [EVK9-MIMX8ULP](../../_boards/evk9mimx8ulp/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [EVK-MIMX8ULP](../../_boards/evkmimx8ulp/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [MIMXRT1180-EVK](../../_boards/evkmimxrt1180/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [EVK-MIMXRT595](../../_boards/evkmimxrt595/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [EVK-MIMXRT685](../../_boards/evkmimxrt685/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [FRDM-MCXN236](../../_boards/frdmmcxn236/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [FRDM-MCXN947](../../_boards/frdmmcxn947/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [FRDM-MCXW71](../../_boards/frdmmcxw71/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [K32W148-EVK](../../_boards/k32w148evk/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- KW45B41Z-LOC
- [KW47-EVK](../../_boards/kw47evk/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [LPCXpresso55S06](../../_boards/lpcxpresso55s06/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [LPCXpresso55S16](../../_boards/lpcxpresso55s16/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [LPCXpresso55S69](../../_boards/lpcxpresso55s69/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [MCIMX93AUTO-EVK](../../_boards/mcimx93autoevk/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [MCIMX93-EVK](../../_boards/mcimx93evk/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [MCIMX93-QSB](../../_boards/mcimx93qsb/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [MCX-N5XX-EVK](../../_boards/mcxn5xxevk/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [MCX-N9XX-EVK](../../_boards/mcxn9xxevk/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../_boards/mimxrt685audevk/trustzone_examples/hello_world/hello_world_s/example_board_readme.md)
