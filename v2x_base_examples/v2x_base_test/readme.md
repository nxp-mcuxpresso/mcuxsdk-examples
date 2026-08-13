# v2x_base_test

## Overview
The V2X base test example is a demonstration program that uses the MCUX SDK
software to exercise the EdgeLock V2X base (debug) services over the V2X
message unit. It brings up the V2X firmware and then runs the base service
operations in sequence:

- get_fw_version - query the V2X firmware version and commit id.
- start_rng - start the V2X TRNG (retried while initialization is in progress).
- pwr_state - request a V2X power state change (power on).
- debug_dump - retrieve the V2X debug dump buffer.

The service protocol is ported from the Linux kernel driver
drivers/firmware/imx/v2x_base_msg.c. It reuses the same S3MU transport and
enclave message framing as the ele_crypto / v2x_crypto components.

## Running the demo
Example output on terminal:

*************** EdgeLock V2X base demo *******************

****************** Bring up V2X firmware *****************
V2X firmware is running.

****************** Get V2X FW version ********************
V2X FW version: 1.0.7  commit 0x60a0acab

****************** Start V2X TRNG ************************
V2X TRNG started.

****************** Set V2X power state ON ****************
V2X power state set to ON.

****************** V2X debug dump ************************
V2X debug dump (20 words):
  [ 0] 0x   101ff
  [ 1] 0x       c
  [ 2] 0x   1008a
  [ 3] 0x       4
  [ 4] 0x   10262
  [ 5] 0x17020402
  [ 6] 0x   10263
  [ 7] 0x8b000000
  [ 8] 0x   10263
  [ 9] 0x     340
  [10] 0x   10263
  [11] 0x8b000000
  [12] 0x   10263
  [13] 0x      20
  [14] 0x   101d3
  [15] 0x     2e0
  [16] 0x   101f3
  [17] 0x       0
  [18] 0x   1008a
  [19] 0xe60011e3

*************** EdgeLock V2X base demo END ***************

## Supported Boards
- [MIMX943-EVK](../../_boards/imx943evk/v2x_base/v2x_base_test/example_board_readme.md)
