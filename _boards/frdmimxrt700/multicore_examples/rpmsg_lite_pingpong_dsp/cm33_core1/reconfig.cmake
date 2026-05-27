#
# Copyright 2025-2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_dsp/hifi1/binary/
    SOURCES dsp_vector_release.bin
            dsp_text_release.bin
            dsp_data_release.bin
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_dsp/hifi1/binary/
    INCLUDES ./
)

# CM33 core1 requires also shared memory for hifi1
mcux_add_linker_symbol(
    SYMBOLS "__use_shmem_hifi1__=1 "
)

mcux_add_iar_configuration(
    LD "--image_input=${SdkRootDirPath}/${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_dsp/hifi1/binary/dsp_vector_release.bin,__dsp_vector_bin,__dsp_vector_section,4\
        --keep=__dsp_vector_bin\
        --image_input=${SdkRootDirPath}/${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_dsp/hifi1/binary/dsp_text_release.bin,__dsp_text_bin,__dsp_text_section,4\
        --keep=__dsp_text_bin\
        --image_input=${SdkRootDirPath}/${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_dsp/hifi1/binary/dsp_data_release.bin,__dsp_data_bin,__dsp_data_section,4\
        --keep=__dsp_data_bin"
)
