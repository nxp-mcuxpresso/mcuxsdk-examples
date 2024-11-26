

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/${board}/cmsis_driver_examples/lpuart/edma_transfer/RTE_Device.h
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/${board}/cmsis_driver_examples/lpuart/edma_transfer
)
