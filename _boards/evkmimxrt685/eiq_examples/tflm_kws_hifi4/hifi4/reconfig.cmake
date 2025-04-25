mcux_add_xtensa_configuration(
  CC "-DXOS_CLOCK_FREQ=594000000 -std=c99"
  CX "-stdlib=libc++ \
    -DCPU_MIMXRT685SFVKB_dsp \
    -std=c++17"
)

mcux_remove_configuration(
  CC "-std=gnu99"
  CX "-std=c++11"
)
