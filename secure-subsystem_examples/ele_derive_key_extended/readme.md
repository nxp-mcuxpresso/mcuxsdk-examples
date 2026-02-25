# ele_derive_key_extended

## Overview
This example showcases key derivation functionality by using SSS API extensions.

It demonstrates how to use the SSS API to do the following operations:
- KDF in counter mode with CMAC as PRF (based on SCP03),
- KDF in counter mode with CMAC as PRF (compatible with NIST 800-108 with CMAC
  as PRF; details in the example comments),
- Miyaguchi-Preneel KDF (based on AUTOSAR SHE1.1),
- HKDF with a single, or multiple output keys (provides better usability for
  multiple automotive and connectivity specifications).

**NOTE** This example depends on the latest loadable firmware released as part of the SDK.

## Running the demo
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ELE Automotive Key Derivation Extensions via SSSAPI Example

**** HDKF key derivation ****
Init IKM key object...OK
Init PRK key object...OK
Init OKM key object...OK
Allocate IKM key handle...OK
Allocate PRK key handle...OK
Allocate OKM key handle...OK
Set IKM...OK
Initialize HKDF SHA256 extract step context...OK
HKDF SHA256 extract...OK
Free HKDF SHA256 extract step context...OK
Initialize HKDF SHA256 expand step context...OK
HKDF SHA256 expand...OK
Get OKM...OK
Derived OKM matches expected OKM...OK
Cleanup...OK

**** HDKF key derivation into multiple key objects ****
Init IKM key object...OK
Init PRK key object...OK
Init OKM1 key object...OK
Init OKM2 key object...OK
Init OKM3 key object...OK
Init OKM4 key object...OK
Init OKM5 key object...OK
Init OKM6 key object...OK
Allocate IKM key handle...OK
Allocate PRK key handle...OK
Allocate OKM1 key handle...OK
Allocate OKM2 key handle...OK
Allocate OKM3 key handle...OK
Allocate OKM4 key handle...OK
Allocate OKM5 key handle...OK
Allocate OKM6 key handle...OK
Set IKM...OK
Initialize HKDF SHA256 extract step context...OK
HKDF SHA256 extract...OK
Free HKDF SHA256 extract step context...OK
Initialize HKDF SHA256 expand step context...OK
HKDF SHA256 expand into six separate, equal-length, OKMs...OK
Get OKM1...OK
Derived OKM matches expected value...OK
Get OKM2...OK
Derived OKM matches expected value...OK
Get OKM3...OK
Derived OKM matches expected value...OK
Get OKM4...OK
Derived OKM matches expected value...OK
Get OKM5...OK
Derived OKM matches expected value...OK
Get OKM6...OK
Derived OKM matches expected value...OK
Cleanup...OK

**** Miyaguchi-Preneel key derivation ****
Init mainKey key object...OK
Init derivedKey key object...OK
Allocate mainKey key handle...OK
Allocate derivedKey key handle...OK
Set mainKey...OK
Initialize MP_KDF context...OK
MP_KDF derivation...OK
Get derived key...OK
Derived key matches expected key...OK
Cleanup...OK

**** CKDF key derivation ****
Init mainKey key object...OK
Init derivedKey key object...OK
Allocate mainKey key handle...OK
Allocate derivedKey key handle...OK
Set mainKey...OK
Initialize CKDF context...OK
CKDF derivation...OK
Get derived key...OK
Derived key matches expected key...OK
Cleanup...OK

**** CKDF SCP03 key derivation ****
Init mainKey key object...OK
Init derivedKey key object...OK
Allocate mainKey key handle...OK
Allocate derivedKey key handle...OK
Set mainKey...OK
Initialize CKDF SCP03 context...OK
CKDF SCP03 derivation...OK
Get derived key...OK
Derived key matches expected key...OK
Cleanup...OK

End of Example with SUCCESS!!

Example end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-MCXW71](../../_boards/frdmmcxw71/secure-subsystem_examples/ele_derive_key_extended/example_board_readme.md)
- [MCX-W71-EVK](../../_boards/mcxw71evk/secure-subsystem_examples/ele_derive_key_extended/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/secure-subsystem_examples/ele_derive_key_extended/example_board_readme.md)
