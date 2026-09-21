# maestro_usb_speaker — mimxrt2660evk

Board-local port of the maestro USB speaker demo for the internal
mimxrt2660evk NPI board. Uses:

- AUDIO__SAI_0 driving the WM8962 codec (U56) for the headphone output
- AUDIO__EDMA3 (edma4 driver) with EDMA channel mux
  (`kDmaRequestMux2AudioSAI0Tx`, no DMAMUX)
- USBHS device stack (single controller) for the USB Audio class
- MODCON `OVERRIDE_IPG_MCLK_EN` gate to keep the SAI0 MCLK output pad
  free-running while the WM8962 is powered on

Validated: build-only under `+armgcc@psram_txt_debug` /
`+armgcc@psram_txt_release`.
