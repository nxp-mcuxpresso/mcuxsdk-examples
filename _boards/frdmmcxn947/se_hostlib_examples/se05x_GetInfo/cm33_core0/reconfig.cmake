
mcux_add_iar_configuration(
    CC "--diag_suppress=Pa082,Pa050,Pe188,Pa089,Pe546,Pa084"
    CX "--diag_suppress=Pa082,Pa050,Pe188,Pa089,Pe546,Pe167,Pe144,Pa183,Pa084"
)
mcux_add_mdk_configuration(
    TARGETS debug
    CC "-O0"
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
    CC "-DSDK_DEBUGCONSOLE_UART\
       -DMCXN\
       -DAX_EMBEDDED\
       -DSIMW_DEMO_ENABLE__DEMO_SE05X_GETINFO\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSDK_DEBUGCONSOLE=1"
)

mcux_remove_iar_configuration(
    CC "--diag_suppress=Pa082,Pa050"
    CX "--diag_suppress=Pa082,Pa050"
)
mcux_remove_mdk_configuration(
    TARGETS debug
    CC "-O1"
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