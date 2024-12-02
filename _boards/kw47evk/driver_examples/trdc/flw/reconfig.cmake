
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/driver_examples/trdc/basic/cm33_core0/pin_mux.c
            examples/_boards/${board}/driver_examples/trdc/basic/cm33_core0/pin_mux.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/trdc/basic/cm33_core0
)
