cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(flexio3_spi_int_lpspi_transfer_master LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/flexio3/spi/int_lpspi_transfer/master)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/flexio/spi/int_lpspi_transfer/master/flexio_spi_int_lpspi_transfer_master.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/flexio3/spi/int_lpspi_transfer/master
             ${board_root}/${board}/driver_examples/flexio3/spi/int_lpspi_transfer/master/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/flexio3/spi/int_lpspi_transfer/master/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)