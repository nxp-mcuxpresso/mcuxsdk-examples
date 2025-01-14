mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES examples/eiq_examples/tflm_cifar10/pcq/
  )
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/eiq_examples/tflm_cifar10/pcq/model_data.h
  examples/eiq_examples/tflm_cifar10/pcq/model_cifarnet_ops_micro.cpp
  )


