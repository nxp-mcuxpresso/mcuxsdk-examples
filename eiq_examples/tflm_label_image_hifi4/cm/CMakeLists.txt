#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tflm_label_image_hifi4 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/eiq_examples/tflm_label_image_hifi4)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
  SOURCES main_cm33.c
)

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/eiq_examples/tflm_label_image_hifi4
  SOURCES dsp_config.h
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/eiq_examples/tflm_label_image_hifi4
  INCLUDES ./
)

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}/examples/eiq_examples/common/
  INCLUDES ./
)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}/examples/eiq_examples/common/
  SOURCES board_init.h
)

mcux_add_macro(
  AS "-DDSP_IMAGE_COPY_TO_RAM=1"
  CC "-DDSP_IMAGE_COPY_TO_RAM=1"
)

include(${SdkRootDirPath}/examples/_boards/${board}/eiq_examples/tflm_label_image_hifi4/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
