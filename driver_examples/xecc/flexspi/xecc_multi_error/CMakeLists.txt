cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(flexspi_xecc_multi_error LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/xecc/flexspi/xecc_multi_error)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/xecc/flexspi/xecc_multi_error/xecc_multi_error.c
            examples/driver_examples/xecc/flexspi/flexspi_xecc_ops.c
            examples/driver_examples/flexspi/nor/polling_transfer/flexspi_nor_flash_ops.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/xecc/flexspi/xecc_multi_error/${core_id}
             ${board_root}/${board}/driver_examples/xecc/flexspi/xecc_multi_error
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/xecc/flexspi/xecc_multi_error/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
