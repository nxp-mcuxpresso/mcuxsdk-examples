cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(executorch_cifarnet_hifi4 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/eiq_examples/executorch_cifarnet_hifi4)
include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_include(
  INCLUDES .
  ../../common
  ../../common/image
  ../../../middleware/eiq
)

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/eiq_examples/common/timer_xtensa.c
  examples/eiq_examples/common/timer_xtensa.h
  examples/eiq_examples/common/timer.h

  # miles edit
  examples/eiq_examples/common/board_init.h
  examples/eiq_examples/common/demo_info.h

  examples/eiq_examples/common/image/image.h
  examples/eiq_examples/common/image/image_decode_raw.c
  examples/eiq_examples/common/image/image_utils.h
  examples/eiq_examples/common/image/image_load.c
  # miles edit

  ${board_root}/${board}/eiq_examples/executorch_cifarnet_hifi4/hifi4/model_pte.h
  examples/eiq_examples/executorch_cifarnet_hifi4/hifi4/demo_config.h
  examples/eiq_examples/executorch_cifarnet_hifi4/hifi4/image_data.h
  examples/eiq_examples/executorch_cifarnet_hifi4/hifi4/labels.h
  examples/eiq_examples/executorch_cifarnet_hifi4/hifi4/main.cpp
  examples/eiq_examples/executorch_cifarnet_hifi4/hifi4/NativeFunctions.h
  examples/eiq_examples/executorch_cifarnet_hifi4/hifi4/RegisterKernels.cpp
)

include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/executorch_cifarnet_hifi4/${core_id}/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/examples/eiq_examples/${core_id}.cmake)
