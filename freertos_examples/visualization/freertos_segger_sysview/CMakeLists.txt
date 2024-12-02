#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(freertos_segger_sysview LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/freertos_examples/visualization/freertos_segger_sysview)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES ./main.c
            ./SEGGER/SEGGER_SYSVIEW_Int.h
            ./SEGGER/SEGGER_SYSVIEW_ConfDefaults.h
            ./SEGGER/SEGGER_SYSVIEW.h
            ./SEGGER/SEGGER_SYSVIEW.c
            ./SEGGER/SEGGER_RTT_printf.c
            ./SEGGER/SEGGER_RTT_ASM_ARMv7M.S
            ./SEGGER/SEGGER_RTT.h
            ./SEGGER/SEGGER_RTT.c
            ./SEGGER/SEGGER.h
            ./Config/SEGGER_SYSVIEW_Conf.h
            ./Config/SEGGER_RTT_Conf.h
            ./Config/Global.h
            ./Sample/FreeRTOSV10/SEGGER_SYSVIEW_FreeRTOS.h
            ./Sample/FreeRTOSV10/SEGGER_SYSVIEW_FreeRTOS.c
)

mcux_add_include(
    INCLUDES .
             ./SEGGER
             ./Sample/FreeRTOSV10
             ./Config
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
