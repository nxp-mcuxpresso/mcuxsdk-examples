
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/mbedtls_examples/mbedtls_benchmark/${core_id}
)

mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
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

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)
