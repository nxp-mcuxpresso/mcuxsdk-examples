# ele_elgamal

## Overview
The ELE Elgamal application provides an example how to load loadable FW into ELE
and how to distingush when FW is loaded, then to import ElGamal blob and print
out the computed shared secret value.

## Running the demo
The following lines are printed to the serial terminal when the demo program is executed.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ELE Elgamal example

Load ELE FW...OK
Initialize and set the local private key...OK
Initialize key object for the shared secret...OK
Import the blob into ELE...OK
Retrieve the shared secret into plaintext buffer...OK
The shared secret is
    99 99 96 A7 89 A3 BC  0 45 C8 A5 FB 42 C7 D1 BD
Computed shared secret matches expected value...OK

Example end
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

## Supported Boards
- [KW47-EVK](../../_boards/kw47evk/secure-subsystem_examples/ele_elgamal/example_board_readme.md)
