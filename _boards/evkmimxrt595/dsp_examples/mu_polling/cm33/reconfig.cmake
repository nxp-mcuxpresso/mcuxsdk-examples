#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/mu_polling/fusionf1/binary/
    SOURCES dsp_reset_release.bin
            dsp_text_release.bin
            dsp_data_release.bin
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/mu_polling/fusionf1/binary/
    INCLUDES ./
)

mcux_add_iar_configuration(
    LD "--image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/mu_polling/fusionf1/binary/dsp_reset_release.bin,__dsp_reset_bin,__dsp_reset_section,4\
        --keep=__dsp_reset_bin\
        --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/mu_polling/fusionf1/binary/dsp_text_release.bin,__dsp_text_bin,__dsp_text_section,4\
        --keep=__dsp_text_bin\
        --image_input=${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/mu_polling/fusionf1/binary/dsp_data_release.bin,__dsp_data_bin,__dsp_data_section,4\
        --keep=__dsp_data_bin"
)
