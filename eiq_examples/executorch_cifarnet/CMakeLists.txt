#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(executorch_cifarnet LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/eiq_examples/executorch_cifarnet)

# Optional ExecuTorch event-tracer profiling. When ON, the example is compiled
# with -DET_EVENT_TRACER_ENABLED and wires an ETDumpGen into the runtime, so the
# inference emits an etdump. It requires the runtime library to have been built
# in profiling mode (build_executorch_runtime.sh --profile); a release/debug .a
# does NOT contain the etdump/flatccrt symbols and will fail to link if this is ON.
option(ETEXAMPLE_ENABLE_PROFILING "Enable ExecuTorch event-tracer profiling (needs profile-mode libexecutorch.a)" OFF)
if(ETEXAMPLE_ENABLE_PROFILING)
  message(STATUS "executorch_cifarnet: profiling ENABLED (-DET_EVENT_TRACER_ENABLED)")
  mcux_add_macro(CC "-DET_EVENT_TRACER_ENABLED" CX "-DET_EVENT_TRACER_ENABLED")
endif()

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

  examples/eiq_examples/executorch_cifarnet/demo_config.h
  examples/eiq_examples/executorch_cifarnet/image_data.h
  examples/eiq_examples/executorch_cifarnet/labels.h
  examples/eiq_examples/executorch_cifarnet/main.cpp
  examples/eiq_examples/executorch_cifarnet/NativeFunctions.h
  examples/eiq_examples/executorch_cifarnet/RegisterKernels.cpp

  examples/eiq_examples/common/board_init.h
)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/executorch_cifarnet/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
