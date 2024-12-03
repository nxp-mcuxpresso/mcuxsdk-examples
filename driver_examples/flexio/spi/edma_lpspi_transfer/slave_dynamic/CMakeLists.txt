cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(flexio_spi_edma_lpspi_transfer_slave_dynamic LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic/flexio_spi_slave_dynamic.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic
             examples/_boards/${board}/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/flexio/spi/edma_lpspi_transfer/slave_dynamic/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)