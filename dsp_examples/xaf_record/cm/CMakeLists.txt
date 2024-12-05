#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dsp_xaf_record LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/dsp_examples/xaf_record)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/dsp_examples/xaf_record/cm/*.c
            examples/dsp_examples/xaf_record/cm/*.h
            examples/dsp_examples/common/mcu/dsp_ipc.c
            examples/dsp_examples/common/mcu/dsp_ipc.h
            ${board_root}/${board}/dsp_examples/xaf_record/dsp_config.h
            ${board_root}/${board}/dsp_examples/srtm_config.h
            ${board_root}/${board}/dsp_examples/srtm_config_audio.h
            ${board_root}/${board}/dsp_examples/rpmsg_config.h
            ${board_root}/${board}/dsp_examples/dsp_support.c
            ${board_root}/${board}/dsp_examples/dsp_support.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/dsp_examples/xaf_record/
    SOURCES incbin.S
    TOOLCHAINS mdk armgcc mcux
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/dsp_examples/common/mcu
             examples/dsp_examples/xaf_record/cm
             ${board_root}/${board}/dsp_examples/xaf_record
             ${board_root}/${board}/dsp_examples
             ${board_root}/${board}/dsp_examples/common/${core_id}
             ${board_root}/${board}/dsp_examples/xaf_record/${core_id}
             ${board_root}/${board}
)


# Add additional configuration
mcux_add_macro(
    CC "-DXA_PCM_GAIN=1\
        -DXA_VIT_PRE_PROC=1\
        -DXA_RENDERER=1\
        -DXA_CAPTURER=1\
        -DVIT_MODELS_ALL=1\
        -DDSP_IMAGE_COPY_TO_RAM=1"
    AS "-DDSP_IMAGE_COPY_TO_RAM=1"
)

include(${SdkRootDirPath}/${board_root}/${board}/dsp_examples/xaf_record/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)