# ele_asymmetric

## Overview
The ELE asymmetric application provides example how to use SSSAPI to perform asymmetric cryptographic operations.

## Running the demo
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ELE Asymmetric via SSSAPI Example

**** ECDSA with P-521 [opaque key] ****
Init key object...OK
Allocate key object handle...OK
Initialize RNG before key generation...OK
Generate P-521 opaque key pair...OK
Init asymmetric context for signing...OK
Sign message digest...OK
Init asymmetric context for verification...OK
Verify signature...OK
Clean up...OK

**** EdDSA with Curve25519 [transparent key] ****
Init key object...OK
Allocate key object handle...OK
Set Curve25519 key pair...OK
Init asymmetric context for signing...OK
Sign message...OK
Init asymmetric context for verification...OK
Verify signature...OK
Clean up...OK

End of Example with SUCCESS!!

Example end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-MCXW71](../../_boards/frdmmcxw71/secure-subsystem_examples/ele_asymmetric/example_board_readme.md)
- [K32W148-EVK](../../_boards/k32w148evk/secure-subsystem_examples/ele_asymmetric/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/secure-subsystem_examples/ele_asymmetric/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/secure-subsystem_examples/ele_asymmetric/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/secure-subsystem_examples/ele_asymmetric/example_board_readme.md)
