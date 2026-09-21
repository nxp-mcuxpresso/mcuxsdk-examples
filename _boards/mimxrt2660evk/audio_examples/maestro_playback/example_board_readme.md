# maestro_playback — MIMXRT2660-EVK board notes

Refer to `examples/audio_examples/maestro_playback/readme.md` for the shared
maestro_playback overview.

## Board setup

- Populate an SD card (FAT32) with the audio files listed in the shared readme
  and insert it into the microSD slot on the MIMXRT2660-EVK.
- Plug headphones or speakers into the WM8962 HP OUT jack.
- Debug console: J-Link OB VCOM at 115200 8N1 (LPUART1 on the board).

## Hardware notes

- Audio codec: WM8962 (U56), driven over LPI2C1.
- I2S: AUDIO__SAI_0 with MCLK sourced from AUDIO PLL / 2 (~24.576 MHz).
- DMA: AUDIO__EDMA3, channel 0, request source `kDmaRequestMux2AudioSAI0Tx`.
- The board-local `streamer_pcm.c` handles the RT2660-specific SAI/EDMA/MODCON
  setup that differs from the shared RT10xx implementation.
