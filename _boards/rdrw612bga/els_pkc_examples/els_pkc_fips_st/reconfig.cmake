
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/els_pkc_examples/els_pkc_fips_st/els_pkc_fips_mbedtls_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/els_pkc_examples/els_pkc_fips_st
)

mcux_add_iar_configuration(
    CX "--diag_suppress Pe546,Pe177"
)
mcux_add_armgcc_configuration(
    CC "-Wno-unused-variable"
)
mcux_add_mdk_configuration(
    TARGETS release
    CC "-O1"
    CX "-O1"
)
mcux_add_mdk_configuration(
    CC "-Wno-tautological-constant-compare"
)

mcux_add_macro(
    CC "-DMBEDTLS_CONFIG_FILE=\\\"els_pkc_fips_mbedtls_config.h\\\""
)

mcux_remove_mdk_configuration(
    TARGETS release
    CC "-Oz"
    CX "-Oz"
)
mcux_remove_armgcc_configuration(
    TARGETS release
    CC "-O3"
    CX "-O3"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x4000\
        --config_def=__heap_size__=0x8000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x4000\"\
        --predefine=\"-D__heap_size__=0x8000\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x4000\
        -Xlinker --defsym=__heap_size__=0x8000"
)
