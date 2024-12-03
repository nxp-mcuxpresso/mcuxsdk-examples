
mcux_add_mdk_configuration(
    TARGETS debug sdram_debug
    CC "-Os"
)
mcux_add_mdk_configuration(
    LD "--diag_suppress 6329"
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DFREESCALE_KSDK_BM\
       -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\""
)

mcux_remove_mdk_configuration(
    TARGETS debug sdram_debug
    CC "-O1"
)

mcux_remove_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O0"
)

mcux_add_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O1"
)