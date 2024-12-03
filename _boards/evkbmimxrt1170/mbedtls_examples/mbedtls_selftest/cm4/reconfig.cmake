
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/mbedtls_examples/mbedtls_selftest
)

mcux_add_iar_configuration(
    TARGETS debug sdram_debug
    CC "-Ohz"
)

mcux_add_macro(
    CC "-DFREESCALE_KSDK_BM\
       -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\""
)

mcux_remove_iar_configuration(
    TARGETS debug sdram_debug
    CC "-On"
)

mcux_remove_iar_configuration(
    TARGETS sdram_debug
    CC "-Om"
)

mcux_remove_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O0"
)

mcux_add_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-Os"
)