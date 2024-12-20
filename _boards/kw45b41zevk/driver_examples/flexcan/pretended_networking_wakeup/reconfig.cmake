
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/flexcan/pretended_networking_wakeup/clock_config.c
    SOURCES ${board_root}/${board}/driver_examples/flexcan/pretended_networking_wakeup/clock_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath} 
    INCLUDES ${board_root}/${board}/driver_examples/flexcan/pretended_networking_wakeup
)