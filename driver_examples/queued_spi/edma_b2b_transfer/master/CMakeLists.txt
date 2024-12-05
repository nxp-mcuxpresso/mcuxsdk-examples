cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(queued_spi_edma_b2b_transfer_master LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/queued_spi/edma_b2b_transfer/master)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/queued_spi/edma_b2b_transfer/master/queued_spi_edma_b2b_transfer_master.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/queued_spi/edma_b2b_transfer/master
             ${board_root}/${board}/driver_examples/queued_spi/edma_b2b_transfer/master/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/queued_spi/edma_b2b_transfer/master/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)