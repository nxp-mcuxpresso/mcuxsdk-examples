# Add additional configuration
mcux_add_macro(
    CC "-DFSL_ETH_ENABLE_CACHE_CONTROL\
        -DETH_MAX_RX_PKTS_AT_ONCE=8\
        -DNETC_RXBD_NUM=8\
        -DNETC_RXBUFF_NUM=16\
        -DTCP_WND=5840\
        -DENET_RXBUFF_NUM=14\
        -DENET_RXBD_NUM=9"
)
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-Og"
)

# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)
