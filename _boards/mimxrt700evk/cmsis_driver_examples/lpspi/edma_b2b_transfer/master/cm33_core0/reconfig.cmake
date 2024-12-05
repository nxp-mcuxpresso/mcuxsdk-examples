mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/cmsis_driver_examples/lpspi/edma_b2b_transfer/master/RTE_Device.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/cmsis_driver_examples/lpspi/edma_b2b_transfer/master/${core_id}/RTE_Device.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/cmsis_driver_examples/lpspi/edma_b2b_transfer/master/${core_id}
)
