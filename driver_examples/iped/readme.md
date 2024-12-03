# iped

## Overview
The IPED Example project is a demonstration program that uses the MCUX SDK software to set up Inline Prince Encryption Decryption.
Then tests the expected behaviour.

## Running the demo
When the demo runs successfully, the terminal displays similar information like the following:
~~~~~~~~~~~~~~~~~~
IPED Peripheral Driver Example

Calling API_Init
API_Init Successfully
Configure IPED region 0 enc/dec: start 0x8002000 end 0x8003000
Configure IPED  Successfully
External Flash memory configured successfully
*Success* read plain data from 0x8001000 to 0x8001FFF
*Success* read programmed&encrypted data from 0x08002000 to 0x8002FFF
*Success* read plain data from 0x8003000 to 0x8003FFF
Disabling IPED
*Success* read plain data from 0x8001000 to 0x8001FFF
*Success* read encrypted data from 0x8002000 to 0x8002FFF
*Success* read plain data from 0x8003000 to 0x8003FFF
Enabling IPED
*Success* read plain data from 0x8001000 to 0x8001FFF
*Success* read decrypted data from 0x8002000 to 0x8002FFF
*Success* read plain data from 0x8003000 to 0x8003FFF
Reconfiguring IPED
Reconfigure IPED  Successfully
*Success* read plain data from 0x8001000 to 0x8001FFF
*Success* read decrypted data from 0x8002000 to 0x8002FFF
*Success* read plain data from 0x8003000 to 0x8003FFF
Disabling IPED
End of example

~~~~~~~~~~~~~~~~~~

Note: Please make sure that flash is cleared and a power cycle has been performed before executing.

## Supported Boards
- [LPCXpresso55S36](../../_boards/lpcxpresso55s36/driver_examples/iped/example_board_readme.md)
