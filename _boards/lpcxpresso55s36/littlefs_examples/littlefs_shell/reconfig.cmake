mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
      examples/_boards/${board}/littlefs_examples/littlefs_shell/peripherals.c
      examples/_boards/${board}/littlefs_examples/littlefs_shell/peripherals.h
)


mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x1000 -Xlinker --defsym=__heap_size__=0x1000"
)