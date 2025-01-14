mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES examples/eiq_examples/tflm_kws/cpu/
  )
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/eiq_examples/tflm_kws/cpu/model_data.h
  examples/eiq_examples/tflm_kws/cpu/model_ds_cnn_ops_micro.cpp
  )


