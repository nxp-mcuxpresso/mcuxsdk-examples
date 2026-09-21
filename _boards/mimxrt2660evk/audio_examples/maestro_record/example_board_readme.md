# maestro_record — MIMXRT2660-EVK board notes

Refer to `examples/audio_examples/maestro_record/readme.md` for the shared
maestro_record overview.

## Board setup

- Populate a microSD card (FAT32) and insert it into the microSD slot on the
  MIMXRT2660-EVK. Recorded WAV files are written to this card.
- The on-board digital microphones (DMIC0) capture the PDM audio. Codec
  playback of recorded content is routed through the WM8962 HP OUT jack.
- Debug console: J-Link OB VCOM at 115200 8N1 (LPUART1 on the board).

## Hardware notes

- Audio codec: WM8962 (U56), driven over LPI2C1.
- I2S: AUDIO__SAI_0 with MCLK sourced from AUDIO PLL / 2 (~24.576 MHz).
- PDM: AUDIO__MICFIL with APPCLK sourced from AUDIO PLL / 2 (~24.576 MHz).
- DMA: AUDIO__EDMA3.
  - Channel 0, request source `kDmaRequestMux2AudioSAI0Tx` (SAI0 TX).
  - Channel 1, request source `kDmaRequestMux2AudioMic` (MICFIL RX).
- The board-local `streamer_pcm.c` handles the RT2660-specific
  SAI/MICFIL/EDMA/MODCON setup that differs from the shared RT10xx
  implementation (which targets SAI1 + DMAMUX + IOMUXC_GPR MCLK gate).
