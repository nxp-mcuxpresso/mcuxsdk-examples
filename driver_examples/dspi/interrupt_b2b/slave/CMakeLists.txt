cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dspi_interrupt_b2b_slave LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/dspi/interrupt_b2b/slave)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/dspi/interrupt_b2b/slave/dspi_interrupt_b2b_slave.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/dspi/interrupt_b2b/slave
             examples/_boards/${board}/driver_examples/dspi/interrupt_b2b/slave/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/dspi/interrupt_b2b/slave/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)