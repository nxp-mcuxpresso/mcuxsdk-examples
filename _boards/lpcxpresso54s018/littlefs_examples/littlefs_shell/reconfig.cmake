mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
      examples/_boards/${board}/littlefs_examples/littlefs_shell/peripherals.c
      examples/_boards/${board}/littlefs_examples/littlefs_shell/peripherals.h
)

mcux_remove_iar_configuration(
    LD "--config_def=__stack_size__=0x1000 --config_def=__heap_size__=0x400"
)

mcux_remove_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x1000\" --predefine=\"-D__heap_size__=0x400\""
)

mcux_remove_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000 -Xlinker --defsym=__heap_size__=0x400"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000 --config_def=__heap_size__=0x1000"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\" --predefine=\"-D__heap_size__=0x1000\""
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000 -Xlinker --defsym=__heap_size__=0x1000"
)
