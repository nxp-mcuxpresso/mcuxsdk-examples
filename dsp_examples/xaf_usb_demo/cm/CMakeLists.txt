#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dsp_xaf_usb_demo LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/dsp_examples/xaf_usb_demo)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/dsp_examples/xaf_usb_demo/cm/main.c
            examples/dsp_examples/xaf_usb_demo/cm/main.h
            examples/dsp_examples/xaf_usb_demo/cm/cmd.c
            examples/dsp_examples/xaf_usb_demo/cm/cmd.h
            examples/dsp_examples/common/mcu/dsp_ipc.c
            examples/dsp_examples/common/mcu/dsp_ipc.h
            examples/_boards/${board}/dsp_examples/xaf_usb_demo/dsp_config.h
            examples/_boards/${board}/dsp_examples/srtm_config.h
            examples/_boards/${board}/dsp_examples/srtm_config_audio.h
            examples/_boards/${board}/dsp_examples/rpmsg_config.h
            examples/dsp_examples/xaf_usb_demo/cm/usb_device_descriptor.h
            examples/dsp_examples/xaf_usb_demo/cm/usb_device_descriptor.c
            examples/dsp_examples/xaf_usb_demo/cm/usb_device_config.h
            examples/dsp_examples/xaf_usb_demo/cm/usb_audio_config.h
            examples/dsp_examples/xaf_usb_demo/cm/composite.h
            examples/dsp_examples/xaf_usb_demo/cm/composite.c
            examples/dsp_examples/xaf_usb_demo/cm/audio_unified.h
            examples/dsp_examples/xaf_usb_demo/cm/audio_unified.c
            examples/_boards/${board}/dsp_examples/dsp_support.c
            examples/_boards/${board}/dsp_examples/dsp_support.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/dsp_examples/common/mcu
             examples/dsp_examples/common
             examples/dsp_examples/xaf_usb_demo/cm
             examples/_boards/${board}/dsp_examples/xaf_usb_demo
             examples/_boards/${board}/dsp_examples
             examples/_boards/${board}/dsp_examples/common/${core_id}
             examples/_boards/${board}/dsp_examples/xaf_usb_demo/${core_id}
             examples/_boards/${board}
)


# Add additional configuration
mcux_add_macro(
    CC "-DXA_CAPTURER=1\
       -DXA_PCM_GAIN=1\
       -DXA_RENDERER\
       -DUSB_STACK_FREERTOS\
       -DDSP_IMAGE_COPY_TO_RAM=1"
    AS "-DDSP_IMAGE_COPY_TO_RAM=1"
)
mcux_add_mdk_configuration(
    CC "-mno-unaligned-access"
)
mcux_add_armgcc_configuration(
    CC "-mno-unaligned-access"
)

include(${SdkRootDirPath}/examples/_boards/${board}/dsp_examples/xaf_usb_demo/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
