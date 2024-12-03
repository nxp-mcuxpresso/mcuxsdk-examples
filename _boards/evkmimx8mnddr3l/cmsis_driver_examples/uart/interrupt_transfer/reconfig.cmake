mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/cmsis_driver_examples/uart/interrupt_transfer/RTE_Device.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8mn/cmsis_driver_examples/uart/interrupt_transfer/pin_mux.c
            examples/_boards/evkmimx8mn/cmsis_driver_examples/uart/interrupt_transfer/pin_mux.h
            examples/_boards/evkmimx8mn/cmsis_driver_examples/uart/interrupt_transfer/hardware_init.c
            examples/_boards/evkmimx8mn/cmsis_driver_examples/uart/interrupt_transfer/app.h
            examples/_boards/evkmimx8mn/cmsis_driver_examples/uart/interrupt_transfer/RTE_Device.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8mn/cmsis_driver_examples/uart/interrupt_transfer
             examples/_boards/evkmimx8mn
)
