# ele_spake2plus

## Overview
The ELE SPAKE2+ application provides an example how to use SSSAPI to derive a shared secret using the SPAKE2+ protocol.

## Running the demo
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ELE SPAKE2+ via SSSAPI Example


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

**** Prepare context and run SPAKE2+ ****
Derive Key Context Init...OK
Run SPAKE2+...OK

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

End of Example with SUCCESS!!

Example end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [KW47-EVK](../../_boards/kw47evk/secure-subsystem_examples/ele_spake2plus/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/secure-subsystem_examples/ele_spake2plus/example_board_readme.md)
