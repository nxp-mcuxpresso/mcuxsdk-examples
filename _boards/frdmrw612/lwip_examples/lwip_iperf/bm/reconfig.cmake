# Add additional configuration
mcux_add_macro(
    CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE\
        -DENET_RXBUFF_NUM=10\
        -DENET_RXBD_NUM=5\
        -DTCP_WND=5840\
        -DPBUF_POOL_SIZE=4"
)
