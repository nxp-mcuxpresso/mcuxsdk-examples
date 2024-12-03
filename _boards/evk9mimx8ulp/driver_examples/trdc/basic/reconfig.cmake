
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/driver_examples/trdc/basic/hardware_init.c
            examples/_boards/evkmimx8ulp/driver_examples/trdc/basic/app.h
            examples/_boards/${board}/driver_examples/trdc/basic/pin_mux.c
            examples/_boards/${board}/driver_examples/trdc/basic/pin_mux.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/driver_examples/trdc/basic
             examples/_boards/${board}/driver_examples/trdc/basic
             examples/_boards/evkmimx8ulp
)
