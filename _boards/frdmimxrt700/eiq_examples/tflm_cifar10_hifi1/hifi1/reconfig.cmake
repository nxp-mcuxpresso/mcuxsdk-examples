mcux_add_xtensa_configuration(
  CC "-DXOS_CLOCK_FREQ=237500000 \
      -std=c99 \
      -ffunction-sections \
      -fdata-sections \
      -fno-exceptions \
      -INLINE:requested \
      -fno-unwind-tables"
  CX "-stdlib=libc++ \
      -ffunction-sections \
      -fdata-sections \
      -fno-exceptions \
      -INLINE:requested \
      -std=c++17"
  LD "-Wl,--gc-sections"
)

mcux_remove_configuration(
  CC "-std=gnu99"
  CX "-std=c++11"
)


