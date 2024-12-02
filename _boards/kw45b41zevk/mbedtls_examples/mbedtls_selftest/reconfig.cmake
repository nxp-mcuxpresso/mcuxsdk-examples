
mcux_add_macro(
    CC "-DMBEDTLS_NXP_SSSAPI\
       -DMBEDTLS_NXP_ELE200\
       -DMBEDTLS_CONFIG_FILE=\\\"sssapi_mbedtls_config.h\\\"\
       -DFREESCALE_KSDK_BM"
)
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS release
    CC "-DSDK_DEBUGCONSOLE_UART"
)
