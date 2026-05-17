mcux_add_xtensa_configuration(
  CC "-DXOS_CLOCK_FREQ=237500000"
  CX "-stdlib=libc++ \
    -std=c++17\
    -DUSE_NPU=1
    "
)
mcux_remove_configuration(
   CC "-std=gnu99"
   CX "-std=c++11"
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
    LD "-mlsp=${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_modelrunner_hifi4/hifi4/linker/min-rt"
)

mcux_add_xtensa_configuration(
    TARGETS debug
    LD "-mlsp=${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_modelrunner_hifi4/hifi4/linker/gdbio"
)
