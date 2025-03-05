#
# Copyright 2025 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_include(
    BASE_PATH ${APPLICATION_BINARY_DIR}/../rpmsg_lite_pingpong_dsp_hifi1/
    INCLUDES ./
)

# CM33 core1 requires also shared memory for hifi1
mcux_add_linker_symbol(
    SYMBOLS "__use_shmem_hifi1__=1 "
)

mcux_add_iar_configuration(
    LD "--image_input=${APPLICATION_BINARY_DIR}/../rpmsg_lite_pingpong_dsp_hifi1/dsp_vector_release.bin,__dsp_vector_bin,__dsp_vector_section,4\
        --keep=__dsp_vector_bin\
        --image_input=${APPLICATION_BINARY_DIR}/../rpmsg_lite_pingpong_dsp_hifi1/dsp_text_release.bin,__dsp_text_bin,__dsp_text_section,4\
        --keep=__dsp_text_bin\
        --image_input=${APPLICATION_BINARY_DIR}/../rpmsg_lite_pingpong_dsp_hifi1/dsp_data_release.bin,__dsp_data_bin,__dsp_data_section,4\
        --keep=__dsp_data_bin"
)
