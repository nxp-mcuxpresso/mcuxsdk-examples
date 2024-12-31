# tpm_timer

## Overview
The TPM project is a simple demonstration program of the SDK TPM driver to use TPM as a timer.
It sets up the TPM hardware block to trigger an interrupt every 1 millisecond.
When the TPM interrupt is triggered 1000 times, a message is printed on the UART terminal.

## Supported Boards
- [IMX943EVK](../../../_boards/imx943evk/driver_examples/tpm/timer_trigger/example_board_readme.md)
