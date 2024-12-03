cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(xspi_octal_polling_transfer LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/xspi/octal/polling_transfer)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/xspi/octal/polling_transfer/xspi_octal_polling_transfer.c
            examples/driver_examples/xspi/octal/polling_transfer/xspi_octal_flash_ops.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/xspi/octal/polling_transfer/${core_id}
             examples/_boards/${board}/driver_examples/xspi/octal/polling_transfer
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/xspi/octal/polling_transfer/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
