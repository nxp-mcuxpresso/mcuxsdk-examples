# mpi_loader_dsp_hello_world

## Overview

The mpi_loader_dsp_hello_world example demonstrates how to add secondary
bootloader in CM33 application to load the multicore packed image, created by
elftosb tool. Once the DSP image is loaded, CM33 can kick off it and both cores
then execute in parallel.

## Supported Boards
- [EVK-MIMXRT595](../../../../_boards/evkmimxrt595/dsp_examples/hello_world_usart/example_board_readme.md)
- [EVK-MIMXRT685](../../../../_boards/evkmimxrt685/dsp_examples/hello_world_usart/example_board_readme.md)
- [MIMXRT685-AUD-EVK](../../../../_boards/mimxrt685audevk/dsp_examples/hello_world_usart/example_board_readme.md)
