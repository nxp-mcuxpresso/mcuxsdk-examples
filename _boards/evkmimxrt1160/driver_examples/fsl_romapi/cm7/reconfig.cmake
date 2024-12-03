
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/driver_examples/fsl_romapi/xip/evkmimxrt1160_flexspi_nor_config.c
            examples/_boards/${board}/driver_examples/fsl_romapi/xip/evkmimxrt1160_flexspi_nor_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/fsl_romapi/xip
)
