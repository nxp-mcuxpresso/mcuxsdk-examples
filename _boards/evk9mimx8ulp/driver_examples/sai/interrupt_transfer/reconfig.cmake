
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/driver_examples/sai/interrupt_transfer/./app.h
            examples/_boards/evkmimx8ulp/driver_examples/sai/interrupt_transfer/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
            examples/driver_examples/sai/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/driver_examples/sai/interrupt_transfer
             examples/_boards/evkmimx8ulp
             examples/driver_examples/sai
)
