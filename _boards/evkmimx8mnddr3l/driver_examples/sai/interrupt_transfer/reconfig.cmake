
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8mn/driver_examples/sai/interrupt_transfer/pin_mux.c
            examples/_boards/evkmimx8mn/driver_examples/sai/interrupt_transfer/pin_mux.h
            examples/_boards/evkmimx8mn/driver_examples/sai/interrupt_transfer/./app.h
            examples/_boards/evkmimx8mn/driver_examples/sai/interrupt_transfer/./hardware_init.c
            middleware/multicore/remoteproc/empty_rsc_table.c
            examples/driver_examples/sai/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8mn/driver_examples/sai/interrupt_transfer
             examples/_boards/evkmimx8mn
             examples/driver_examples/sai
)

mcux_add_armgcc_configuration(
    TARGETS flash_release
    CC "-O2"
)

mcux_remove_armgcc_configuration(
    TARGETS flash_release
    CC "-Os"
)
