
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/trdc/basic/cm33_core0/pin_mux.c
            ${board_root}/${board}/driver_examples/trdc/basic/cm33_core0/pin_mux.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/trdc/basic/cm33_core0
)
