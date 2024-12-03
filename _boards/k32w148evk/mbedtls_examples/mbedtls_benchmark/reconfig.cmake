
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/kw45b41zevk/mbedtls_examples/mbedtls_benchmark/hardware_init.c
            examples/_boards/kw45b41zevk/mbedtls_examples/mbedtls_benchmark/app.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/kw45b41zevk/mbedtls_examples/mbedtls_benchmark
)

mcux_add_mdk_configuration(
    LD "--diag_suppress 6329"
)

mcux_add_macro(
    CC "-DMBEDTLS_NXP_SSSAPI\
       -DMBEDTLS_NXP_ELE200\
       -DMBEDTLS_CONFIG_FILE=\\\"sssapi_mbedtls_config.h\\\"\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DFREESCALE_KSDK_BM"
)
