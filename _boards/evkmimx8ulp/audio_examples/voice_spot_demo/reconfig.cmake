
mcux_add_macro(
    CC "-DUSE_PDM_EDMA=1\
        -DENABLE_IDLE_CALLBACK=1"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x14000\
        -Xlinker --defsym=__heap_size__=0x10000"
)
