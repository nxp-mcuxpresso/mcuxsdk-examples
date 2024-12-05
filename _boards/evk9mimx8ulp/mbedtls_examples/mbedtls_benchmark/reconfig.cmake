
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/evkmimx8ulp/mbedtls_examples/mbedtls_benchmark/hardware_init.c
            ${board_root}/evkmimx8ulp/mbedtls_examples/mbedtls_benchmark/app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/evkmimx8ulp/mbedtls_examples/mbedtls_benchmark
             ${board_root}/evkmimx8ulp
)

mcux_add_mdk_configuration(
    LD "--diag_suppress 6329"
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DFREESCALE_KSDK_BM\
       -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\""
)
