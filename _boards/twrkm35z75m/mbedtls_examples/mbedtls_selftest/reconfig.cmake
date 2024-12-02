
mcux_add_macro(
    CC "-DFREESCALE_KSDK_BM\
       -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\""
)
mcux_add_macro(
    TOOLCHAINS mdk
    CC "-DMULADDC_CANNOT_USE_R7\
       -DMBEDTLS_NO_UDBL_DIVISION"
)
