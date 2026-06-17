# ce_lite_nonblockingmode_test_freertos

## Overview
This application demonstrates the non-blocking mode operation of the ZV2118 DSPV-Lite
accelerator using the LCE (Lightweight CE) component with a FreeRTOS OSA backend.

Two FreeRTOS tasks are created, both performing a 32-point complex F16 FFT
(CFFT 32 F16) via `LCE_TransformCFFT_F16`. The CE command buffer is initialized with
`kCE_CmdModeOneNonBlocking`, so the CM33 does not busy-wait for ZV2118 to finish each
command. Instead, the LCE component handles synchronization internally, and the FreeRTOS
preemptive scheduler allows the two tasks to run concurrently while waiting for CE
completion, providing better CPU utilization than the bare-metal variant.

Each task validates its result by computing the normalized squared error between the
computed output and the reference output, and reports pass/fail over the serial console.

## Supported Boards
- [FRDM-KW43](../../../_boards/frdmkw43/ce_lite_examples/nonblockingmode_test/freertos/example_board_readme.md)
- [FRDM-MCXW70](../../../_boards/frdmmcxw70/ce_lite_examples/nonblockingmode_test/freertos/example_board_readme.md)
