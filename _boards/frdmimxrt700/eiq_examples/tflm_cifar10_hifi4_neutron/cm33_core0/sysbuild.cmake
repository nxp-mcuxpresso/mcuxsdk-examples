#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    SOURCES ../hifi4/binary/dsp_literal_release.bin
            ../hifi4/binary/dsp_text_release.bin
            ../hifi4/binary/dsp_data_release.bin
            ../hifi4/binary/dsp_literal_debug.bin
            ../hifi4/binary/dsp_text_debug.bin
            ../hifi4/binary/dsp_data_debug.bin
)

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_cifar10_hifi4_neutron
  SOURCES dsp_support.h
  dsp_support.c
)

mcux_add_include(
    INCLUDES ../hifi4/binary
)

mcux_add_source(
    SOURCES ../incbin.S
    TOOLCHAINS mdk armgcc mcux
)

mcux_add_macro(
    TARGETS flash_debug
    AS "-DDEBUG"
)

mcux_add_macro(
    CC "-DDSP_IMAGE_COPY_TO_RAM=1\
    -DEIQ_EXAMPLE_HSRUN_CLOCK"
)

mcux_add_iar_configuration(
    TARGETS release flash_release
    LD "--image_input=${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_cifar10_hifi4_neutron/hifi4/binary/dsp_literal_release.bin,__dsp_literal_bin,__dsp_literal_section,4\
        --keep=__dsp_literal_bin\
        --image_input=${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_cifar10_hifi4_neutron/hifi4/binary/dsp_text_release.bin,__dsp_text_bin,__dsp_text_section,4\
        --keep=__dsp_text_bin\
        --image_input=${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_cifar10_hifi4_neutron/hifi4/binary/dsp_data_release.bin,__dsp_data_bin,__dsp_data_section,4\
        --keep=__dsp_data_bin"
)

mcux_add_iar_configuration(
    TARGETS debug flash_debug
    LD "--image_input=${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_cifar10_hifi4_neutron/hifi4/binary/dsp_literal_debug.bin,__dsp_literal_bin,__dsp_literal_section,4\
        --keep=__dsp_literal_bin\
        --image_input=${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_cifar10_hifi4_neutron/hifi4/binary/dsp_text_debug.bin,__dsp_text_bin,__dsp_text_section,4\
        --keep=__dsp_text_bin\
        --image_input=${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_cifar10_hifi4_neutron/hifi4/binary/dsp_data_debug.bin,__dsp_data_bin,__dsp_data_section,4\
        --keep=__dsp_data_bin"
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../tflm_cifar10_hifi4_neutron/${CONFIG_TOOLCHAIN}/
)