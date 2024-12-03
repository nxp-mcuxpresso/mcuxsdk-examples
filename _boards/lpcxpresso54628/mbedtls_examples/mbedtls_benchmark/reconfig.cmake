
mcux_add_armgcc_configuration(
    CC "-fno-strict-aliasing"
)
mcux_add_iar_configuration(
    CX "--diag_suppress Pe188"
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DFREESCALE_KSDK_BM\
       -DMBEDTLS_CONFIG_FILE=\\\"ksdk_mbedtls_config.h\\\""
)
