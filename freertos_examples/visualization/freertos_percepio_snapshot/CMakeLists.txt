#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(freertos_percepio_snapshot LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/freertos_examples/visualization/freertos_percepio_snapshot)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES main.c
            trcConfig.h
            GenericRecorderLibSrc/Include/trcBase.h
            GenericRecorderLibSrc/Include/trcHardwarePort.h
            GenericRecorderLibSrc/Include/trcKernel.h
            GenericRecorderLibSrc/Include/trcKernelHooks.h
            GenericRecorderLibSrc/Include/trcKernelPort.h
            GenericRecorderLibSrc/Include/trcTypes.h
            GenericRecorderLibSrc/Include/trcUser.h
            GenericRecorderLibSrc/readme.txt
#            GenericRecorderLibSrc/Release notes.txt
            GenericRecorderLibSrc/trcBase.c
            GenericRecorderLibSrc/trcHardwarePort.c
            GenericRecorderLibSrc/trcKernel.c
            GenericRecorderLibSrc/trcKernelPortFreeRTOS.c
            GenericRecorderLibSrc/trcUser.c
)

mcux_add_include(
    INCLUDES .
             GenericRecorderLibSrc/Include
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

