# ele_derive_key

## Overview
The ELE derive key application provides example how to use SSSAPI to derive keys by
utilising the HKDF functionality, as well as how to derive keys for an Elliptic Curve
Diffie-Hellman key exchange operation.

## Running the demo
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ELE Derive Key via SSSAPI Example

**** Elliptic Curve Diffie-Hellman ****
Init Alice key object...OK
Init Bob key object...OK
Init shared key object...OK
Allocate Alice key object handle...OK
Allocate Bob key object handle...OK
Allocate shared key object handle...OK
Initialize RNG before key generation...OK
Import Alice key pair...OK
Import Bob public key...OK
Init Derive Key Context...OK
ECDH key derivation...OK
Get derived shared secret...OK
Derived shared secret matches expected shared secret...OK
Cleanup...OK

**** HDKF key derivation ****
Init salt key object...OK
Init PRK key object...OK
Init OKM key object...OK
Allocate salt key handle...OK
Allocate PRK key handle...OK
Allocate OKM key handle...OK
Set salt...OK
Initialize HKDF SHA256 extract step context...OK
HKDF SHA256 extract...OK
Free HKDF SHA256 extract step context...OK
Initialize HKDF SHA256 expand step context...OK
HKDF SHA256 expand...OK
Get OKM...OK
Derived OKM matches expected OKM...OK
Cleanup...OK

End of Example with SUCCESS!!

Example end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Note
By default, IAR's "Static clustering" optimization option is enabled in the IAR `release` targets.
Compile flag `--no_clustering` has been added to the build to disable static clustering due to it causing erroneous execution of this example.

## Supported Boards
- [KW47-EVK](../../_boards/kw47evk/secure-subsystem_examples/ele_derive_key/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/secure-subsystem_examples/ele_derive_key/example_board_readme.md)
- [KW47-LOC](../../_boards/kw47loc/secure-subsystem_examples/ele_derive_key/example_board_readme.md)
- [MCXW72-LOC](../../_boards/mcxw72loc/secure-subsystem_examples/ele_derive_key/example_board_readme.md)
