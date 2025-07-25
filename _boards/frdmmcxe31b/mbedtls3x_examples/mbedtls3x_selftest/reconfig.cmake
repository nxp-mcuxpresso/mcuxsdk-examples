
mcux_add_macro(
    CC "-DFRDM_MCXE247\
        -DFREEDOM"
)

mcux_add_macro(
    CC "-DMBEDTLS_CONFIG_FILE=\\\"mcux_mbedtls_config.h\\\""
    CC "-DMCUXPRESSO_SDK"
)
