# Add additional configuration
mcux_add_macro(
    CC "-DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\"\
        -DPBUF_POOL_SIZE=5\
        -DENET_RXBD_NUM=5\
        -DENET_RXBUFF_NUM=14"
)
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1\
       -g1"
    CX "-g1"
)

# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0\
       -g"
    CX "-g"
)
