cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(enet_txrx_rxinterrupt LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/enet/txrx_rxinterrupt)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lpc_enet/txrx_rxinterrupt/enet_txrx_rxinterrupt.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/enet/txrx_rxinterrupt
             ${board_root}/${board}/driver_examples/enet/txrx_rxinterrupt/${core_id}
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1"
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/enet/txrx_rxinterrupt/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)