cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tflm_label_image_ext_mem LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/eiq_examples/tflm_label_image_ext_mem)

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
  examples/eiq_examples/common/timer.h
  examples/eiq_examples/common/timer.c

  examples/eiq_examples/tflm_label_image_ext_mem/main.cpp

  examples/eiq_examples/common/image/image.h
  examples/eiq_examples/common/board_init.h
  examples/eiq_examples/common/image/image_decode_raw.c
  examples/eiq_examples/common/image/image_utils.h
  examples/eiq_examples/common/image/image_load.c

  examples/eiq_examples/tflm_label_image_ext_mem/image_data.h
  examples/eiq_examples/tflm_label_image_ext_mem/demo_config.h
  examples/eiq_examples/tflm_label_image_ext_mem/labels.h
  examples/eiq_examples/tflm_label_image_ext_mem/grace_hopper.bmp
  examples/eiq_examples/common/tflm/model/output_postproc.h
  examples/eiq_examples/common/tflm/model/output_postproc.cpp

  examples/driver_examples/xspi/octal/edma_transfer/xspi_octal_flash_edma_ops.c
  )

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/eiq_examples/tflm_label_image_ext_mem/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
