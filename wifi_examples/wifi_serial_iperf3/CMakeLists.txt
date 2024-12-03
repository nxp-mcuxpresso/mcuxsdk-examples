#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(wifi_serial_iperf3 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/wifi_examples/wifi_serial_iperf3)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/lwip_examples/iperf3/main.c
            examples/lwip_examples/iperf3/iperf_api.h
            examples/lwip_examples/iperf3/port/iperf_mwm.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/lwip_examples/iperf3
)

# Add additional configuration
mcux_add_macro(
    CC "-DIPERF3_WIFI=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DSCANF_ADVANCED_ENABLE=1\
       -DDEBUG_CONSOLE_ENABLE_ECHO=1\
       -DSDK_DEBUGCONSOLE=1"
)


include(${SdkRootDirPath}/examples/_boards/${board}/wifi_examples/wifi_serial_iperf3/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)