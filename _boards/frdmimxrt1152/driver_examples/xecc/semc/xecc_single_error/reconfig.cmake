
mcux_add_macro(
    CC "-DUSE_SDRAM"
)
mcux_add_macro(
    TOOLCHAINS iar
    CC "-DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ=0"
)
