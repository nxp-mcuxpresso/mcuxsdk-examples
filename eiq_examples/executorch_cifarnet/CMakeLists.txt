#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(executorch_cifarnet LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/eiq_examples/executorch_cifarnet)

mcux_add_include(
  INCLUDES .
  ../common
  ../common/image
  ../../../middleware/eiq
)

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/eiq_examples/common/timer.c
  examples/eiq_examples/common/timer.h

  examples/eiq_examples/executorch_cifarnet/image_data.h
  examples/eiq_examples/executorch_cifarnet/main.cpp
  examples/eiq_examples/executorch_cifarnet/NativeFunctions.h
  examples/eiq_examples/executorch_cifarnet/RegisterKernels.cpp

  examples/eiq_examples/common/board_init.h
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/executorch_cifarnet/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
