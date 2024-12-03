
mcux_add_mdk_configuration(
    CC "-Wno-implicit\
       -Os"
)

mcux_add_macro(
    CC "-DDEMO_MMCAU_PASS_RATE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DPRINTF_ADVANCED_ENABLE=1"
)
