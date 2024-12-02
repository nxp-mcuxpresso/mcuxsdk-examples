cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tflm_kws LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/eiq_examples/tflm_kws)

include(${SdkRootDirPath}/CMakeLists.txt)
include(${SdkRootDirPath}/examples/_boards/${board}/eiq_examples/tflm_kws/reconfig.cmake OPTIONAL)

mcux_add_include(
  INCLUDES .
  ../common
  ../common/audio
  ../common/model
  ../common/tflm/audio
  ../common/tflm/model
  )

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/eiq_examples/common/tflm/model/get_top_n.cpp
  examples/eiq_examples/common/board_init.h
  examples/eiq_examples/common/tflm/model/get_top_n.h
  examples/eiq_examples/common/tflm/audio/kws_mfcc.cpp
  examples/eiq_examples/common/tflm/audio/kws_mfcc.hpp
  examples/eiq_examples/common/tflm/audio/mfcc.cpp
  examples/eiq_examples/common/tflm/audio/mfcc.hpp
  examples/eiq_examples/common/tflm/model.cpp
  examples/eiq_examples/common/tflm/model/model.h
  examples/eiq_examples/common/tflm/demo_info.cpp
  examples/eiq_examples/common/demo_info.h
  examples/eiq_examples/tflm_kws/output_postproc.cpp
  examples/eiq_examples/common/tflm/model/output_postproc.h
  examples/eiq_examples/tflm_kws/main.cpp
  examples/eiq_examples/common/audio/audio_load.cpp
  examples/eiq_examples/common/audio/audio.h
  examples/eiq_examples/tflm_kws/audio_data.h
  examples/eiq_examples/tflm_kws/demo_config.h
  examples/eiq_examples/tflm_kws/labels.h
  examples/eiq_examples/tflm_kws/off.wav

  examples/eiq_examples/tflm_kws/train.py
  examples/eiq_examples/common/timer.c
  examples/eiq_examples/common/timer.h
  )

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
