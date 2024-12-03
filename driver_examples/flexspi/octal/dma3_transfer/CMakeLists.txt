cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(flexspi_octal_dma3_transfer LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/flexspi/octal/edma_transfer)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/flexspi/octal/dma3_transfer/flexspi_octal_edma_transfer.c
            examples/driver_examples/flexspi/octal/dma3_transfer/flexspi_octal_flash_ops.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/flexspi/octal/edma_transfer/${core_id}
             examples/_boards/${board}/driver_examples/flexspi/octal/edma_transfer
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/flexspi/octal/edma_transfer/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)