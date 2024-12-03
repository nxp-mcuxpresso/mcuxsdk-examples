# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ezhv_linear2tiledYUV_primary_core LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/ezhv_examples/linear2tiledYUV)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/ezhv_examples/
    SOURCES linear2tiledYUV/primary_core/linear2tile_primary_core.c
            linear2tiledYUV/primary_core/jpeg.c
            linear2tiledYUV/primary_core/jpeg.h
            ezhv_firmware/ezhv_para.h
            ezhv_firmware/ezhv_api.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/ezhv_examples/ezhv_firmware
)

mcux_add_include(
    BASE_PATH ${APPLICATION_BINARY_DIR}/../binary
    INCLUDES ./
    TOOLCHAINS mdk armgcc
)

include(${SdkRootDirPath}/examples/_boards/${board}/ezhv_examples/linear2tiledYUV/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

mcux_add_iar_configuration(
    LD "--image_input=${APPLICATION_BINARY_DIR}/../binary/ezhv_image.bin,_ezhv_image,__ezhv_image,4 \
        --keep _ezhv_image"
)

mcux_add_mdk_configuration(
    LD "--keep=*(*ezhv_code)"
)
