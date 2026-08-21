#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(executorch_modelrunner LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/eiq_examples/executorch_modelrunner)

mcux_add_include(
  INCLUDES .
  ../common
  ../../../middleware/eiq
)

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/eiq_examples/common/timer.c
          examples/eiq_examples/common/timer.h
          examples/eiq_examples/common/board_init.h
          examples/eiq_examples/executorch_modelrunner/main.cpp
          examples/eiq_examples/executorch_modelrunner/NativeFunctions.h
          examples/eiq_examples/executorch_modelrunner/RegisterKernels.cpp
          examples/eiq_examples/executorch_modelrunner/protocol.h
          examples/eiq_examples/executorch_modelrunner/protocol.cpp
          examples/eiq_examples/executorch_modelrunner/serial_protocol.h
          examples/eiq_examples/executorch_modelrunner/serial_protocol.cpp
          examples/eiq_examples/executorch_modelrunner/executorch_engine.h
          examples/eiq_examples/executorch_modelrunner/executorch_engine.cpp
          examples/eiq_examples/executorch_modelrunner/command_handler.h
          examples/eiq_examples/executorch_modelrunner/command_handler.cpp
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/executorch_modelrunner/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
