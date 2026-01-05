mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES ${board_root}/${board}/eiq_examples/tflm_cifar10/pcq/
  )

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES ${board_root}/${board}/eiq_examples/tflm_cifar10/pcq/model_data.h
  ${board_root}/${board}/eiq_examples/tflm_cifar10/pcq/model_cifarnet_ops_micro.cpp
  )
mcux_remove_mdk_configuration(
	CC "-fshort-wchar"
	CX "-fshort-wchar"
	)

