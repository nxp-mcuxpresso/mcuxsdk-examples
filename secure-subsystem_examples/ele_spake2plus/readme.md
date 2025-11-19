# ele_spake2plus

## Overview
The ELE SPAKE2+ application provides an example how to use SSSAPI to derive a shared secret using the SPAKE2+ protocol.

## Running the demo
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ELE SPAKE2+ via SSSAPI Example


**** RUNNING SPAKE2+ (Matter) ****

**** Initialize all key objects ****
Init mainKey key object...OK
Init w0 key object...OK
Init L key object...OK
Init pA key object...OK
Init pB key object...OK
Init cA key object...OK
Init cB key object...OK
Init Ke key object...OK

**** Allocate handles ****
Allocate mainKey handle...OK
Allocate w0 handle...OK
Allocate L handle...OK
Allocate pA handle...OK
Allocate pB handle...OK
Allocate cA handle...OK
Allocate cB handle...OK
Allocate Ke handle...OK

**** Set the inputs ****
Set mainKey key-pair...OK
Set the w0 value...OK
Set the L registration record...OK
Set the pA public share...OK

**** Prepare context and run SPAKE2+ (Matter) ****
Derive Key Context Init...OK
Run SPAKE2+ (Matter)...OK

**** Check against expected values ****
Get the Ke key...OK
Check against KeRef...OK
Get the pB key...OK
Check against pBRef...OK
Get the cA key...OK
Check against cARef...OK
Get the cB key...OK
Check against cBRef...OK
Clean up...OK


**** RUNNING SPAKE2+ (CCC) ****

**** Initialize all key objects ****
Init mainKey key object...OK
Init w0 key object...OK
Init L key object...OK
Init pA key object...OK
Init pB key object...OK
Init cA key object...OK
Init cB key object...OK
Init Ke key object...OK
Init kenc key object...OK
Init kmac key object...OK
Init krmac key object...OK
Init ltss key object...OK
Init kBleIntr key object...OK
Init kBleOob key object...OK

**** Allocate handles ****
Allocate mainKey handle...OK
Allocate w0 handle...OK
Allocate L handle...OK
Allocate pA handle...OK
Allocate pB handle...OK
Allocate cA handle...OK
Allocate cB handle...OK
Allocate Ke handle...OK
Allocate kenc handle...OK
Allocate kmac handle...OK
Allocate krmac handle...OK
Allocate ltss handle...OK
Allocate kBleIntr handle...OK
Allocate kBleOob handle...OK

**** Set the inputs ****
Set mainKey key-pair...OK
Set the w0 value...OK
Set the L registration record...OK
Set the pA public share...OK

**** Prepare context and run SPAKE2+ (CCC)****
Derive Key Context Init...OK
Run SPAKE2+ (CCC)...OK

**** Check against expected values ****
Get the Ke (SK) key...OK
Check against KeRef...OK
Get the pB (Y) key...OK
Check against pBRef...OK
Get the cA (M1) key...OK
Check against cARef...OK
Get the cB (M2) key...OK
Check against cBRef...OK
Get the kenc key...OK
Check against kencRef...OK
Get the kmac key...OK
Check against kmacRef...OK
Get the krmac key...OK
Check against krmacRef...OK
Get the ltss key...OK
Check against ltssRef...OK
Get the kBleIntr key...OK
Check against kBleIntrRef...OK
Get the kBleOob key...OK
Check against kBleOobRef...OK
Clean up...OK

End of Example with SUCCESS!!

Example end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [KW47-EVK](../../_boards/kw47evk/secure-subsystem_examples/ele_spake2plus/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/secure-subsystem_examples/ele_spake2plus/example_board_readme.md)
- [KW47-LOC](../../_boards/kw47loc/secure-subsystem_examples/ele_spake2plus/example_board_readme.md)
- [MCXW72-LOC](../../_boards/mcxw72loc/secure-subsystem_examples/ele_spake2plus/example_board_readme.md)
- [FRDM-MCXW72](../../_boards/frdmmcxw72/secure-subsystem_examples/ele_spake2plus/example_board_readme.md)
