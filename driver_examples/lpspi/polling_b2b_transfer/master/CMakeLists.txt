cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lpspi_polling_b2b_transfer_master LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/lpspi/polling_b2b_transfer/master)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lpspi/polling_b2b_transfer/master/lpspi_polling_b2b_transfer_master.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/lpspi/polling_b2b_transfer/master
             examples/_boards/${board}/driver_examples/lpspi/polling_b2b_transfer/master/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/lpspi/polling_b2b_transfer/master/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)