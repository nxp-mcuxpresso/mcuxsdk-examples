mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/cmsis_driver_examples/lpuart/edma_transfer/RTE_Device.h
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/cmsis_driver_examples/lpuart/edma_transfer/${core_id}/RTE_Device.h
)


mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/cmsis_driver_examples/lpuart/edma_transfer/${core_id}
)
