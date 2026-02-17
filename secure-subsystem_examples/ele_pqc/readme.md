# ele_pqc

## Overview
The ELE PQC application provides an example how to use SSSAPI to perform post-quantum cryptography operations on the S200 Secure Subsystem.

Usage of the PQC API is dependent on the latest firmware provided for the secure subsystem via the MCUX SDK.

## Running the demo
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ELE PQC via SSSAPI Example


**** ML-DSA-65 SHA3-256 Pre-Hashed Sign and Verify ****
****     without Context and External Work Area    ****
Initialize keyPair key object...OK
Allocate keyPair key object...OK
Generate key...OK
Initialize asymmetric signature context...OK
Sign pre-hashed message...OK
Clear the signature context...OK
Initialize asymmetric verification context...OK
Verify pre-hashed message...OK
Clean up...OK


****    ML-DSA-87 Pure Sign and Verify   ****
**** with Context and External Work Area ****
Initialize keyPair key object...OK
Allocate keyPair key object...OK
Generate key...OK
Initialize asymmetric signature context...OK
Sign message...OK
Clear the signature context...OK
Initialize asymmetric verification context...OK
Verify message...OK
Clean up...OK


**** ML-KEM-1024 Establish Shared Secret ****
Initialize keyPair key object...OK
Initialize Alice's shared secret key object...OK
Initialize Bob's shared secret key object...OK
Allocate keyPair key object...OK
Allocate Alice's shared secret key object...OK
Allocate Bob's shared secret key object...OK
Generate ML-KEM key pair...OK
Initialize ML-KEM key derivation context...OK
ML-KEM Encapsulate on Bob's side first...OK
ML-KEM Decapsulate on Alice's side...OK
Get the shared secret key K (Bob's)...OK
Get the shared secret key K' (Alice's)...OK
Verify that K' == K...OK
Clean up...OK

End of Example with SUCCESS!!

Example end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [KW47-EVK](../../_boards/kw47evk/secure-subsystem_examples/ele_pqc/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/secure-subsystem_examples/ele_pqc/example_board_readme.md)
- [KW47-LOC](../../_boards/kw47loc/secure-subsystem_examples/ele_pqc/example_board_readme.md)
- [MCXW72-LOC](../../_boards/mcxw72loc/secure-subsystem_examples/ele_pqc/example_board_readme.md)
- [FRDM-MCXW72](../../_boards/frdmmcxw72/secure-subsystem_examples/ele_pqc/example_board_readme.md)
