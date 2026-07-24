mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/xspi/psram/edma_transfer/xspi_psram_edma_ops.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/xspi/psram/edma_transfer/xspi_psram_edma_ops.c
)
