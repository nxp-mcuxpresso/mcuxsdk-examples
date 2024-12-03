
mcux_add_mdk_configuration(
    LD "--diag_suppress 6329"
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DFREESCALE_KSDK_BM\
       -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\"\
       -DFRDM_K32L2A4S\
       -DFREEDOM"
)
mcux_add_macro(
    TOOLCHAINS mdk
    CC "-DMULADDC_CANNOT_USE_R7"
)
