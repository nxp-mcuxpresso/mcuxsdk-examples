mcux_add_macro(
    CC "-DMBEDTLS_CONFIG_FILE=\\\"mcux_mbedtls_config.h\\\""
    CC "-DMCUXPRESSO_SDK"
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
