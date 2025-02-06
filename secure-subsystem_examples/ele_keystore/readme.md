# ele_symmetric

## Overview
The ELE keystore application provides example how to use SSSAPI to export key into blob and later import it back.

## Running the demo
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ELE Keystore via SSSAPI Example

Initialize RNG for opaque key generation...OK
Generate opaque key...OK
Encrypt message by AES CBC using opaque key...OK
Export opaque key as blob...OK
Delete key from keystore...OK
Free all object, close keystore and session...OK
Open session and keystore...OK
Import blob into ELE...OK
Decryption previously encrypted message...OK
Decrypted message matches with plaintext, that means imported and exported key is the same.

End of Example with SUCCESS!!

Example end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [FRDM-MCXW71](../../_boards/frdmmcxw71/secure-subsystem_examples/ele_symmetric/example_board_readme.md)
- [K32W148-EVK](../../_boards/k32w148evk/secure-subsystem_examples/ele_symmetric/example_board_readme.md)
- [KW45B41Z-EVK](../../_boards/kw45b41zevk/secure-subsystem_examples/ele_symmetric/example_board_readme.md)
- [KW47-EVK](../../_boards/kw47evk/secure-subsystem_examples/ele_symmetric/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/secure-subsystem_examples/ele_symmetric/example_board_readme.md)
