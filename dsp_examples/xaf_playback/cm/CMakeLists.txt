#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dsp_xaf_playback LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/dsp_examples/xaf_playback)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/dsp_examples/xaf_playback/cm/*.c
            examples/dsp_examples/xaf_playback/cm/*.h
            examples/dsp_examples/xaf_playback/resources/*.h
            examples/dsp_examples/common/mcu/*.c
            examples/dsp_examples/common/mcu/*.h
            examples/_boards/${board}/sdmmc_config.c
            examples/_boards/${board}/sdmmc_config.h
            examples/_boards/${board}/dsp_examples/*.c
            examples/_boards/${board}/dsp_examples/*.h
            examples/_boards/${board}/dsp_examples/xaf_playback/*.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/dsp_examples/common/mcu
             examples/dsp_examples/xaf_playback/cm
             examples/dsp_examples/xaf_playback/resources
             examples/_boards/${board}
             examples/_boards/${board}/dsp_examples
             examples/_boards/${board}/dsp_examples/xaf_playback
             examples/_boards/${board}/dsp_examples/xaf_playback/${core_id}
)

# Add additional configuration
mcux_add_macro(
    CC "-DXA_MP3_DECODER=1\
       -DXA_AAC_DECODER=1\
       -DXA_OPUS_ENCODER=1\
       -DXA_OPUS_DECODER=1\
       -DXA_SBC_ENCODER=1\
       -DXA_SBC_DECODER=1\
       -DXA_VORBIS_DECODER=1\
       -DXA_PCM_GAIN=1\
       -DXA_SRC_PP_FX=1\
       -DXA_RENDERER=1\
       -DDSP_IMAGE_COPY_TO_RAM=1"
    AS "-DDSP_IMAGE_COPY_TO_RAM=1"
)

mcux_add_configuration(
    CC "-D__USE_SHMEM"
)

mcux_add_linker_symbol(
    SYMBOLS "__use_shmem__=1"
)

include(${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_playback/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)