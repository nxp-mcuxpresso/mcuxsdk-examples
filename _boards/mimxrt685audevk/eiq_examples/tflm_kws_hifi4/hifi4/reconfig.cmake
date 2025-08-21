mcux_add_xtensa_configuration(
  CC "-DXOS_CLOCK_FREQ=237500000 -mlongcalls -std=c99"
  CX "-stdlib=libc++ \
    -DCPU_MIMXRT685SFVKB_dsp \
    -mlongcalls \
    -std=c++17"
)

mcux_remove_configuration(
  CC "-std=gnu99"
  CX "-std=c++11"
)
