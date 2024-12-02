cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(cmsis_enet_txrx_transfer LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/cmsis_driver_examples/enet/txrx_transfer)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/cmsis_driver_examples/enet/txrx_transfer/cmsis_enet_txrx_transfer.c
            examples/_boards/${board}/cmsis_driver_examples/enet/txrx_transfer/RTE_Device.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/cmsis_driver_examples/enet/txrx_transfer
             examples/_boards/${board}/cmsis_driver_examples/enet/txrx_transfer/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/cmsis_driver_examples/enet/txrx_transfer/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)