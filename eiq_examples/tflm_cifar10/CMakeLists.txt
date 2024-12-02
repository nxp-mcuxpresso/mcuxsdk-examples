cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tflm_cifar10 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/eiq_examples/tflm_cifar10)

mcux_add_include(
  INCLUDES .
  ../common
  ../common/image
  ../common/tflm/model
  )

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/eiq_examples/common/tflm/model/get_top_n.cpp
  examples/eiq_examples/common/tflm/model/get_top_n.h
  examples/eiq_examples/common/tflm/model.cpp
  examples/eiq_examples/common/tflm/model/model.h
  examples/eiq_examples/common/tflm/demo_info.cpp
  examples/eiq_examples/common/demo_info.h
  examples/eiq_examples/common/timer.c
  examples/eiq_examples/common/timer.h

  examples/eiq_examples/tflm_cifar10/main.cpp

  examples/eiq_examples/common/board_init.h
  examples/eiq_examples/common/image/image.h
  examples/eiq_examples/common/image/image_decode_raw.c
  examples/eiq_examples/common/image/image_utils.h
  examples/eiq_examples/common/image/image_load.c

  examples/eiq_examples/tflm_cifar10/image_data.h
  examples/eiq_examples/tflm_cifar10/labels.h
  examples/eiq_examples/tflm_cifar10/ship.bmp
  examples/eiq_examples/tflm_cifar10/demo_config.h
  examples/eiq_examples/common/tflm/model/output_postproc.h
  examples/eiq_examples/common/tflm/model/output_postproc.cpp
  )

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/eiq_examples/tflm_cifar10/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
