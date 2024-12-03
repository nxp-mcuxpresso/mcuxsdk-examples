# ce_fft_test

## Overview
This application is a test vehicle for interfacing CM33 with ZV2117. This implements:
1. Basic loopback test
Send a "NULL" command from CM33 to ZV. ZV simply reads the command and returns. Status[0] echoes the command, Status[1] writes "0x5CCE55" to indicate "success"

2. Data loopback test
Send a "COPY" command from CM33 to ZV. ZV copies source buffer to destination buffer based on arguments send from CM33. Status[0] echoes the command, Status[1] writes "0x5CCE55" to indicate "success".

cmd buffer is placed at MEM address: 0x20028000
status buffer is placed at MEM address: 0x20028400

This test is based on memory polling from both sides (CM33 and ZV)

## Supported Boards
- [KW47-EVK](../../_boards/kw47evk/ce_examples/fft_test/example_board_readme.md)
- [KW47-LOC](../../_boards/kw47loc/ce_examples/fft_test/example_board_readme.md)
- [MCX-W72-EVK](../../_boards/mcxw72evk/ce_examples/fft_test/example_board_readme.md)
- [FRDM-MCXW72](../../_boards/frdmmcxw72/ce_examples/fft_test/example_board_readme.md)
