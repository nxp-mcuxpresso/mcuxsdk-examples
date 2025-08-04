
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples_int/_boards/${board}/emwin_examples/common/pin_mux.c
            examples_int/_boards/${board}/emwin_examples/common/pin_mux.h
            examples_int/_boards/${board}/emwin_examples/common/hardware_init.c
            examples_int/_boards/${board}/emwin_examples/common/app.h
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x1000"
)
mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x1000\""
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000"
)
