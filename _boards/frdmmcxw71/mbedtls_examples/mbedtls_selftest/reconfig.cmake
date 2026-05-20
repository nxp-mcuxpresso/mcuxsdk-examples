
mcux_add_macro(
    CC "-DMBEDTLS_NXP_SSSAPI\
       -DMBEDTLS_NXP_ELE200\
       -DFREESCALE_KSDK_BM"
)
mcux_add_macro(
    TOOLCHAINS iar
    TARGETS release
    CC "-DSDK_DEBUGCONSOLE_UART"
)
