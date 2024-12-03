
mcux_add_macro(
    CC "-DFREESCALE_KSDK_BM\
       -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\""
)

mcux_remove_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O0"
)

mcux_add_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O1"
)

mcux_remove_iar_configuration(
    TARGETS sdram_debug
    CC "-On"
)

mcux_add_iar_configuration(
    TARGETS sdram_debug
    CC "-Ol"
)