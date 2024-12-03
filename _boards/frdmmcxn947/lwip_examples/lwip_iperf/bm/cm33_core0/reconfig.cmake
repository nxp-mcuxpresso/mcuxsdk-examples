# Add additional configuration
mcux_add_macro(
    CC "-DENET_RXBD_NUM=14\
        -DTCP_WND=18980"
)
