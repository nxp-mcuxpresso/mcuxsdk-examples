
mcux_add_macro(
    CC "-DFREESCALE_KSDK_BM\
       -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\""
)

mcux_add_iar_configuration(
    LD "--config_def=__heap_size__=0x4000"
)

mcux_remove_mdk_configuration(
    LD "--predefine=\"-D__heap_size__=0x8000\""
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__heap_size__=0x4000\""
)

mcux_remove_armgcc_configuration(
    LD "-Xlinker --defsym=__heap_size__=0x8000"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__heap_size__=0x4000"
)
