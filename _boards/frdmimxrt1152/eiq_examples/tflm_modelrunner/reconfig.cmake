mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/eiq_examples/common/
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/common/timer.c
            examples/eiq_examples/common/timer.h
)

mcux_remove_mdk_configuration(
    CC "-fshort-wchar"
    CX "-fshort-wchar"
)

mcux_add_mdk_configuration(
    CX "-std=gnu++17"
)
mcux_add_mdk_configuration(
    CC "-Wno-format -Wno-enum-compare-switch"
    CX "-Wno-format -Wno-enum-compare-switch"
)
mcux_add_iar_configuration(
    LD "--redirect _Printf=_PrintfFull"
)
mcux_add_mdk_configuration(
    LD "--predefine=-D__stack_size__=0x4000\
       --predefine=-D__heap_size__=0x20000\
       --library_type=standardlib\
       --diag_suppress=6439,6776"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x4000\
        --config_def=__heap_size__=0x20000"
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__heap_size__=0x20000\
        -Xlinker --defsym=__stack_size__=0x4000"
)

mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
       -DARM_MATH_CM7\
       -D__FPU_PRESENT=1\
       -DMODEL_SIZE=100*1024\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1"
    CX "-DSDK_DEBUGCONSOLE_UART\
       -DARM_MATH_CM7\
       -D__FPU_PRESENT=1\
       -DMODEL_SIZE=100*1024\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1"
)
