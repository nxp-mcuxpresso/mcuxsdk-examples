
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/mbedtls_examples/mbedtls_selftest/hardware_init.c
            examples/_boards/evkmimx8ulp/mbedtls_examples/mbedtls_selftest/app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/mbedtls_examples/mbedtls_selftest
             examples/_boards/evkmimx8ulp
)

mcux_add_macro(
    CC "-DFREESCALE_KSDK_BM\
       -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\""
)
