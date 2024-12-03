cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(flexspi_xecc_single_error LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/xecc/flexspi/xecc_single_error)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/xecc/flexspi/xecc_single_error/xecc_single_error.c
            examples/driver_examples/xecc/flexspi/xecc_single_error/flexspi_nor_flash_ops.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/xecc/flexspi/xecc_single_error/${core_id}
             examples/_boards/${board}/driver_examples/xecc/flexspi/xecc_single_error
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/xecc/flexspi/xecc_single_error/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)