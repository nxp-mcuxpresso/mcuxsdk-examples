# Copyright 2024-2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lwip_https_client_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/lwip_examples/lwip_https_client/freertos)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/lwip_examples/lwip_https_client/freertos/lwip_https_client_freertos.c
            examples/lwip_examples/lwip_https_client/freertos/app_ca_bundle.h
            examples/lwip_examples/lwip_https_client/freertos/app_ca_bundle.c
            examples/lwip_examples/lwip_https_client/freertos/app_corehttp.h
            examples/lwip_examples/lwip_https_client/freertos/app_corehttp.c
            examples/lwip_examples/lwip_https_client/freertos/app_mbedtls_socket.h
            examples/lwip_examples/lwip_https_client/freertos/app_mbedtls_socket.c
            examples/lwip_examples/lwip_https_client/freertos/app_https_client_task.h
            examples/lwip_examples/lwip_https_client/freertos/app_https_client_task.c
            examples/lwip_examples/lwip_https_client/freertos/core_http_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/lwip_examples/lwip_https_client/freertos
)

mcux_add_macro(
    CC "-DUSE_RTOS=1\
        -DPRINTF_ADVANCED_ENABLE=1\
        -DMBEDTLS_ERROR_C"
)

mcux_add_macro(
    TOOLCHAINS armgcc
    CC "-D_POSIX_SOURCE"
)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x400\
             __heap_size__=0x18000"
)

include(${SdkRootDirPath}/${board_root}/${board}/lwip_examples/lwip_https_client/freertos/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)