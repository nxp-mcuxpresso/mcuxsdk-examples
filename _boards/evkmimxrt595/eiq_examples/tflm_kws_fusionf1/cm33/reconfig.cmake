#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    SOURCES ../fusionf1/binary/dsp_literal_release.bin
            ../fusionf1/binary/dsp_text_release.bin
            ../fusionf1/binary/dsp_data_release.bin
)

mcux_add_include(
    INCLUDES ../fusionf1/binary
)

mcux_add_macro(
    CC "-DDSP_IMAGE_COPY_TO_RAM=1"
)

mcux_add_iar_configuration(
    LD "--image_input=${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_kws_fusionf1/fusionf1/binary/dsp_literal_release.bin,__dsp_literal_bin,__dsp_literal_section,4\
        --keep=__dsp_literal_bin\
        --image_input=${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_kws_fusionf1/fusionf1/binary/dsp_text_release.bin,__dsp_text_bin,__dsp_text_section,4\
        --keep=__dsp_text_bin\
        --image_input=${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_kws_fusionf1/fusionf1/binary/dsp_data_release.bin,__dsp_data_bin,__dsp_data_section,4\
        --keep=__dsp_data_bin"
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../tflm_kws_fusionf1/${CONFIG_TOOLCHAIN}/
)
