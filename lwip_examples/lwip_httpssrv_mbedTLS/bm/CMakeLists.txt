cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lwip_httpssrv_mbedTLS_bm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/lwip_examples/lwip_httpssrv_mbedTLS/bm)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/lwip_examples/lwip_httpssrv_mbedTLS/bm/httpd_mbedtls.c
            examples/lwip_examples/lwip_httpssrv_mbedTLS/bm/httpd_mbedtls.h
            examples/lwip_examples/lwip_httpssrv_mbedTLS/bm/lwip_httpssrv_mbedTLS_bm.c
            examples/lwip_examples/lwip_httpssrv_mbedTLS/bm/lwipopts.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/lwip_examples/lwip_httpssrv_mbedTLS/bm
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=1000\
             __heap_size__=40000"
)

include(${SdkRootDirPath}/examples/_boards/${board}/lwip_examples/lwip_httpssrv_mbedTLS/bm/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)