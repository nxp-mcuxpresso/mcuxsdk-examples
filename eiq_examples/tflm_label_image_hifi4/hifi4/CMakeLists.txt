cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tflm_label_image_hifi4 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/eiq_examples/tflm_label_image_hifi4)
include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_include(
  INCLUDES .
  ../../common
  ../../common/image
  ../../common/tflm/model
  ../../tflm_label_image_hifi4/hifi4/pcq/
)

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/eiq_examples/common/tflm/model/get_top_n.cpp
  examples/eiq_examples/common/tflm/model/get_top_n.h
  examples/eiq_examples/common/tflm/model.cpp
  examples/eiq_examples/common/tflm/model/model.h
  examples/eiq_examples/common/tflm/demo_info.cpp
  examples/eiq_examples/common/demo_info.h
  examples/eiq_examples/common/timer_xtensa.c
  examples/eiq_examples/common/timer_xtensa.h
  examples/eiq_examples/common/timer.h
  examples/eiq_examples/tflm_label_image_hifi4/hifi4/pcq/model_data.h
  examples/eiq_examples/tflm_label_image_hifi4/hifi4/pcq/model_mobilenet_ops_micro.cpp

  examples/eiq_examples/tflm_label_image_hifi4/hifi4/main.cpp

  examples/eiq_examples/common/board_init.h
  examples/eiq_examples/common/image/image.h
  examples/eiq_examples/common/image/image_decode_raw.c
  examples/eiq_examples/common/image/image_utils.h
  examples/eiq_examples/common/image/image_load.c

  examples/eiq_examples/tflm_label_image_hifi4/hifi4/image_data.h
  examples/eiq_examples/tflm_label_image_hifi4/hifi4/labels.h
  examples/eiq_examples/tflm_label_image_hifi4/hifi4/stopwatch.bmp
  examples/eiq_examples/tflm_label_image_hifi4/hifi4/demo_config.h
  examples/eiq_examples/common/tflm/model/output_postproc.h
  examples/eiq_examples/common/tflm/model/output_postproc.cpp
)

include(${SdkRootDirPath}/examples/_boards/${board}/eiq_examples/tflm_label_image_hifi4/${core_id}/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/examples/eiq_examples/${core_id}.cmake)
