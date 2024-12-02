cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(flexspi_hyper_ram_polling_transfer LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/flexspi/hyper_ram/polling_transfer)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/flexspi/hyper_ram/polling_transfer/flexspi_hyper_ram_polling_transfer.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/flexspi/hyper_ram/polling_transfer
             examples/_boards/${board}/driver_examples/flexspi/hyper_ram/polling_transfer/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/flexspi/hyper_ram/polling_transfer/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)