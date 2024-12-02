mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/cmsis_driver_examples/lpuart/edma_transfer/RTE_Device.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/kw45b41zevk/cmsis_driver_examples/lpuart/edma_transfer/hardware_init.c
            examples/_boards/kw45b41zevk/cmsis_driver_examples/lpuart/edma_transfer/app.h
            examples/_boards/kw45b41zevk/cmsis_driver_examples/lpuart/edma_transfer/RTE_Device.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/kw45b41zevk/cmsis_driver_examples/lpuart/edma_transfer
)
