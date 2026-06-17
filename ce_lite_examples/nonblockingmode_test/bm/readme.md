# ce_lite_nonblockingmode_test_bm

## Overview
This application demonstrates the non-blocking mode operation of the ZV2118 DSPV-Lite
accelerator using the LCE (Lightweight CE) component with a bare-metal OSA backend.

Two bare-metal OSA tasks are created, both performing a 512-point complex F32 FFT
(CFFT 512 F32) via `LCE_TransformCFFT_F32`. The CE command buffer is initialized with
`kCE_CmdModeOneNonBlocking`, so the CM33 does not busy-wait for ZV2118 to finish each
command. Instead, the LCE component handles synchronization internally, and the BM OSA
cooperative scheduler allows the two tasks to interleave while waiting for CE completion.

Each task validates its result by computing the normalized squared error between the
computed output and the reference output, and reports pass/fail over the serial console.

## Supported Boards
- [FRDM-KW43](../../../_boards/frdmkw43/ce_lite_examples/nonblockingmode_test/bm/example_board_readme.md)
- [FRDM-MCXW70](../../../_boards/frdmmcxw70/ce_lite_examples/nonblockingmode_test/bm/example_board_readme.md)
