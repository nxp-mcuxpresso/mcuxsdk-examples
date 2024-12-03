
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lwip_httpsrv_enet_qos_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/lwip_examples/lwip_httpsrv_enet_qos/freertos)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/lwip_examples/httpsrv_common/freertos/CMakeLists.txt)
include(${SdkRootDirPath}/examples/lwip_examples/httpsrv_common/fs_data/CMakeLists.txt)
include(${SdkRootDirPath}/examples/lwip_examples/httpsrv_common/fs_data/webpage/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/lwip_examples/lwip_httpsrv/freertos/lwip_httpsrv_freertos.c
            examples/lwip_examples/lwip_httpsrv/freertos/lwipopts.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/lwip_examples/lwip_httpsrv/freertos             
)

mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DHTTPSRV_CFG_WEBSOCKET_ENABLED=1"
)

mcux_add_macro(
    TOOLCHAINS armgcc
    CC "-D_POSIX_SOURCE"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=2048\
             __heap_size__=60000"
)

include(${SdkRootDirPath}/examples/_boards/${board}/lwip_examples/lwip_httpsrv_enet_qos/freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)