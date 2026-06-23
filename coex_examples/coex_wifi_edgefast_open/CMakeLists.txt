#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(coex_wifi_edgefast_open LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/coex_examples/coex_wifi_edgefast_open)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/coex_examples/coex_wifi_edgefast_open
    TARGET edgefast_open_app.h
)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/coex_examples/coex_wifi_edgefast_open/${multicore_foldername}/edgefast_open_app.h
    CONFIG True
    PREINCLUDE True
    PREINCLUDE_TYPE c_include cpp_include
)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
    examples/coex_examples/coex_wifi_edgefast_open/main.c
    middleware/edgefast_open/examples/shell/shell.c
    middleware/edgefast_open/examples/shell/shell.h
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES
    examples/coex_examples/coex_wifi_edgefast_open
    middleware/edgefast_open/examples/shell
    middleware/wifi_nxp/example/wifi_cli
    middleware/wifi_nxp/example/wifi_cli/wifi_config
    middleware/wifi_nxp/example/wifi_cli/lwip_config
)

mcux_add_macro(
  CC "-DLPUART_RING_BUFFER_SIZE=1024U\
      -DPRINTF_ADVANCED_ENABLE=1\
      -DSDK_DEBUGCONSOLE_UART=1\
      -DSDK_OS_FREE_RTOS\
      -DSHELL_TASK_STACK_SIZE=4096\
      -DUSE_RTOS=1\
      -DCOEX_APP_SUPPORT=1"
)

mcux_add_iar_configuration(
    CC "--no_inline"
    CX "--no_clustering"
    LD "--semihosting"
)

mcux_add_mdk_configuration(
    LD "--library_type=microlib"
)

include(${SdkRootDirPath}/${board_root}/${board}/coex_examples/coex_wifi_edgefast_open/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
