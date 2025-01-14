mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES examples/eiq_examples/tflm_cifar10/pcq/
   examples/eiq_examples/common/
  )
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/eiq_examples/tflm_cifar10/pcq/model_data.h
   examples/eiq_examples/common/timer.c
    examples/eiq_examples/common/timer.h
  examples/eiq_examples/tflm_cifar10/pcq/model_cifarnet_ops_micro.cpp
  )


