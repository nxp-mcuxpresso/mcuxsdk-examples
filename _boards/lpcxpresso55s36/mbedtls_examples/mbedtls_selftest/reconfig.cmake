
mcux_add_iar_configuration(
    CX "--diag_suppress Pe188"
)
mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-O1"
)

mcux_add_iar_configuration(
    LD "--config_def __pkc__=1"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__pkc__=1\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__pkc__=1"
)

mcux_add_macro(
    CC "-DMBEDTLS_CONFIG_FILE=\\\"els_pkc_mbedtls_config.h\\\"\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -DFREESCALE_KSDK_BM"
)

mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)
