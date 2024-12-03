
mcux_add_mdk_configuration(
    TARGETS release
    CC "-O2"
    CX "-O2"
)
mcux_add_iar_configuration(
    CC "--diag_suppress=Pa082,Pa050,Pe188,Pa089,Pe546,Pa084"
    CX "--diag_suppress=Pa082,Pa050,Pe188,Pa089,Pe546,Pe167,Pe144,Pa183,Pa084"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/se_hostlib/demos/ksdk/se05x/default/fsl_sss_ftr.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/se_hostlib/demos/ksdk/se05x/default
)

mcux_add_macro(
    CC "-DLPC_55x=1\
       -DSDK_DEBUGCONSOLE_UART\
       -DPRINTF_ADVANCED_ENABLE=1"
)

mcux_remove_mdk_configuration(
    TARGETS release
    CC "-O0"
    CX "-O0"
)
mcux_remove_iar_configuration(
    CC "--diag_suppress=Pa082,Pa050"
    CX "--diag_suppress=Pa082,Pa050"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x3000 \
       --config_def=__heap_size__=0x4000"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x3000\" \
        --predefine=\"-D__heap_size__=0x4000\""
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x3000 \
        -Xlinker --defsym=__heap_size__=0x4000"
)
