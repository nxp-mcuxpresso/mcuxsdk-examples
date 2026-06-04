mcux_add_xtensa_configuration(
  CC "-DXOS_CLOCK_FREQ=237500000 -std=c99"
  CX "-stdlib=libc++ \
    -std=c++17"
)

mcux_remove_configuration(
  CC "-std=gnu99"
  CX "-std=c++11"
)