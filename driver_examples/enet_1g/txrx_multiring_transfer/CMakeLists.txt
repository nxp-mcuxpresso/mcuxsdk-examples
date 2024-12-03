cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(enet_1g_txrx_multiring_transfer LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/enet_1g/txrx_multiring_transfer)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/enet/txrx_multiring_transfer/enet_txrx_multiring_transfer.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/enet_1g/txrx_multiring_transfer
             examples/_boards/${board}/driver_examples/enet_1g/txrx_multiring_transfer/${core_id}
)

mcux_add_macro(
    CC "-DENET_ENHANCEDBUFFERDESCRIPTOR_MODE\
       -DPRINTF_ADVANCED_ENABLE=1"
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/enet_1g/txrx_multiring_transfer/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)