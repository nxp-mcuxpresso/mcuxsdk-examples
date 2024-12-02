# ele_digest

## Overview
The ELE digest application provides example how to use SSSAPI to generate hash digests and MACs and how to use multiple contexts with the S200 subsystem.

## Running the demo
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ELE Digest via SSSAPI Example

**** SHA256 One-Go ****
Init digest context...OK
Generate digest with one-go operation...OK
Check if digest matches expected value...OK
Free digest context...OK

**** SHA1/256/384/512 Multi-Part with Context Switching ****
Init contexts for all four digests...OK

Init SHA1...OK
Init SHA256...OK
Init SHA384...OK
Init SHA512...OK

Update SHA1   with first 176 bytes...OK
Update SHA256 with first 176 bytes...OK
Update SHA384 with first 176 bytes...OK
Update SHA512 with first 176 bytes...OK

Update SHA512 with remaining 176 bytes...OK
Update SHA384 with remaining 176 bytes...OK
Update SHA256 with remaining 176 bytes...OK
Update SHA1   with remaining 176 bytes...OK

Finish SHA1...OK
Check SHA1 digest...OK
Finish SHA256...OK
Check SHA256 digest...OK
Finish SHA384...OK
Check SHA384 digest...OK
Finish SHA512...OK
Check SHA512 digest...OK

Free contexts of all four digests...OK

**** Opaque and transparent One-Go CMAC / HMAC  ****
Init CMAC key object...OK
Init HMAC key object...OK
Allocate CMAC key handle...OK
Allocate HMAC key handle...OK
Initialize RNG for opaque key generation...OK

Set transparent CMAC key...OK
Set transparent HMAC key...OK
Init CMAC context...OK
Init HMAC context...OK
Generate CMAC keyed digest...OK
Generate HMAC keyed digest...OK
Check if CMAC correct...OK
Check if HMAC correct...OK

Generate opaque CMAC key...OK
Generate opaque HMAC key...OK
Init CMAC context...OK
Init HMAC context...OK
Generate CMAC keyed digest...OK
Generate HMAC keyed digest...OK

Free contexts and key objects...OK

End of Example with SUCCESS!!

Example end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-MCXW71](../../_boards/frdmmcxw71/secure-subsystem_examples/ele_digest/example_board_readme.md)
- [K32W148-EVK](../../_boards/k32w148evk/secure-subsystem_examples/ele_digest/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/secure-subsystem_examples/ele_digest/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/secure-subsystem_examples/ele_digest/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/secure-subsystem_examples/ele_digest/example_board_readme.md)
