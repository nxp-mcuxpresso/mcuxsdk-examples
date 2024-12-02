# dsp_pdm_sai

## Overview
The dsp_pdm_sai demo demonstrates audio processing using DSP core.
It uses the PDM and SAI driver to capture and playback the audio.
One channel audio data is converted to samples in the PDM module.
Then, the data is placed into the memory buffer by DMA. Last, it is sent to the I2S
buffer and output to the CODEC, where the audio data will be heard from lineout.

## Supported Boards
- [EVK9-MIMX8ULP](../../_boards/evk9mimx8ulp/dsp_examples/pdm_sai/example_board_readme.md)
- [EVK-MIMX8ULP](../../_boards/evkmimx8ulp/dsp_examples/pdm_sai/example_board_readme.md)
