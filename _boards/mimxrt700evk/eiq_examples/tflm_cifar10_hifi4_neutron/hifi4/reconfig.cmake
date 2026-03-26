mcux_add_xtensa_configuration(
  CC "-std=c99"
  CX "-stdlib=libc++ \
    -std=c++17"
)
mcux_add_xtensa_configuration(
  TARGETS debug release
  CC "-DTENSORARENA_NONCACHE=1"
  CX "-DTENSORARENA_NONCACHE=1"
)

mcux_remove_configuration(
  CC "-std=gnu99"
  CX "-std=c++11"
)

mcux_add_include(
  INCLUDES .
  ../../../../../../middleware/eiq/neutron/common/include
  ../../../../../../middleware/eiq/neutron/driver/include
  )
mcux_add_source(
  SOURCES model_cifarnet_ops_npu.cpp
  model_data.h
  ../../../../../../middleware/eiq/neutron/rt700/hifi4/libNeutronFirmware.a
  ../../../../../../middleware/eiq/neutron/rt700/hifi4/libNeutronDriver.a
  )

mcux_remove_configuration(
    TARGETS release
    LD "-mlsp=${XTENSA_LSP_RELEASE}"
)

mcux_remove_configuration(
    TARGETS debug
    LD "-mlsp=${XTENSA_LSP_DEBUG}"
)

mcux_add_xtensa_configuration(
    TARGETS release
    LD "-mlsp=${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_cifar10_hifi4_neutron/linker/hifi4/min-rt"
)

mcux_add_xtensa_configuration(
    TARGETS debug
    LD "-mlsp=${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_cifar10_hifi4_neutron/linker/hifi4/gdbio"
)
