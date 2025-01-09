# memfault

## Overview

This project is used to validate the memfault firmware SDK integration. More information
of memfault can be found here: https://memfault.com/

This project runs the self test cases from memfault firmware SDK, to test whether the
integration is OK. If test pass, the log `Test successed` is shown at the end of
output log.

Please note that only flash targets are supported!

## Running the project

This project doesn't need interactive operations, the log is shown in the terminal.
The log shows `Test successed` if all test cases pass, otherwise it shows
`Test failed`.

## Supported Boards
- [FRDM-RW612](../../_boards/frdmrw612/component_examples/memfault/example_board_readme.md)
