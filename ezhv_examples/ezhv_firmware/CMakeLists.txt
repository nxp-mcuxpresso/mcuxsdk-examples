# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ezhv_firmware_ezhv LANGUAGES C CXX ASM)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    SOURCES ezhv_main.c
            ezhv_camera_dvp.c
            ezhv_linear2tile.c
            ezhv_mip.c
			ezhv_api.h
			ezhv_para.h
)

mcux_add_macro(
    CC "-DEZHV_DVP_EN=1 \
	    -DEZHV_LINEAR_TO_TILE_EN=1 \
		-DEZHV_MIP_EN=1 "
)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/ezhv_image.bin TOOLCHAINS riscvllvm)

mcux_add_custom_command(
    BUILD_EVENT POST_BUILD
    TOOLCHAINS riscvllvm
    BUILD_COMMAND 
    ${CMAKE_COMMAND} -E copy ${APPLICATION_BINARY_DIR}/ezhv_image.bin ../binary/ezhv_image.bin
)
