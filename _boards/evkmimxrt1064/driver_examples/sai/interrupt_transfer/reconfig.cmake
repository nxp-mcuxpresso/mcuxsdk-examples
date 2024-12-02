
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/sai/interrupt_transfer/music/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/driver_examples/sai/interrupt_transfer/music
)

mcux_add_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O1"
)

mcux_remove_armgcc_configuration(
    TARGETS debug sdram_debug
    CC "-O0"
)
