cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tflm_kws_fusionf1 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/eiq_examples/tflm_kws_fusionf1)
include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_include(
  INCLUDES .
  ../../common
  ../../common/audio
  ../../common/tflm/model
  ../../common/tflm/audio
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
  examples/eiq_examples/common/tflm/model/output_postproc.h
  examples/eiq_examples/common/audio/audio_load.cpp
  examples/eiq_examples/common/audio/audio.h
  examples/eiq_examples/tflm_kws_fusionf1/fusionf1/main.cpp
  examples/eiq_examples/tflm_kws_fusionf1/fusionf1/output_postproc.cpp
  examples/eiq_examples/tflm_kws_fusionf1/fusionf1/audio_data.h
  examples/eiq_examples/tflm_kws_fusionf1/fusionf1/demo_config.h
  examples/eiq_examples/tflm_kws_fusionf1/fusionf1/labels.h
  examples/eiq_examples/tflm_kws_fusionf1/fusionf1/model_data.h
  examples/eiq_examples/tflm_kws_fusionf1/fusionf1/model_ds_cnn_ops_micro.cpp
  examples/eiq_examples/common/board_init.h
  examples/eiq_examples/common/timer_xtensa.c
  examples/eiq_examples/common/timer.h
  examples/eiq_examples/tflm_kws_fusionf1/fusionf1/train.py
  examples/eiq_examples/tflm_kws_fusionf1/fusionf1/off.wav

  )

include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_kws_fusionf1/${core_id}/reconfig.cmake OPTIONAL)

include(${SdkRootDirPath}/examples/eiq_examples/${core_id}.cmake)
