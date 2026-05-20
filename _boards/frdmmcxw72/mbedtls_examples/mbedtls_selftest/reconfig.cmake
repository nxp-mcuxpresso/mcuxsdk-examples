
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/mbedtls_examples/mbedtls_selftest/${core_id}
)

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
