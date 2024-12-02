
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x4000\
        --config_def=__heap_size__=0x8000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x4000\"\
        --predefine=\"-D__heap_size__=0x8000\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x4000\
        -Xlinker --defsym=__heap_size__=0x8000"
)


    mcux_add_macro(
        CC  "PSA_CRYPTO_DRIVER_ELE_S2XX"
    )
