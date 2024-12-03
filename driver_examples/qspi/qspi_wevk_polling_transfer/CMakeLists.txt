cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(qspi_wevk_polling_transfer LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/qspi/wevk_polling_transfer)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/qspi/polling_transfer/qspi_polling_transfer.c
            examples/driver_examples/qspi/polling_transfer/qspi_nor_flash_ops.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/qspi/wevk_polling_transfer/${core_id}
             examples/_boards/${board}/driver_examples/qspi/wevk_polling_transfer
             examples/_boards/${board}/driver_examples/qspi/polling_transfer/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/qspi/wevk_polling_transfer/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)