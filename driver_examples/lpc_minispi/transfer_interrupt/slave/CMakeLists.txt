cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(spi_transfer_interrupt_slave LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/spi/transfer_interrupt/slave)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lpc_minispi/transfer_interrupt/slave/spi_transfer_interrupt_slave.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/spi/transfer_interrupt/slave
             examples/_boards/${board}/driver_examples/spi/transfer_interrupt/slave/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/spi/transfer_interrupt/slave/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)