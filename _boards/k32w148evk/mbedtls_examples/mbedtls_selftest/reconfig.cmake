
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/kw45b41zevk/mbedtls_examples/mbedtls_selftest/hardware_init.c
            ${board_root}/kw45b41zevk/mbedtls_examples/mbedtls_selftest/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/kw45b41zevk/mbedtls_examples/mbedtls_selftest
)

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
