mcux_add_xtensa_configuration(
  CC "-DXOS_CLOCK_FREQ=237500000 -std=c99"
  CX "-stdlib=libc++ \
    -DCPU_MIMXRT798SGFOA_hifi4 \
    -std=c++17"
)

mcux_remove_configuration(
  CC "-std=gnu99"
  CX "-std=c++11"
)

#mcux_remove_configuration(
#    TARGETS debug
#    LD "-mlsp=${XTENSA_LSP_DEBUG}"
#)
#mcux_remove_configuration(
#    TARGETS release
#    LD "-mlsp=${XTENSA_LSP_RELEASE}"
#)
#
#mcux_add_xtensa_configuration(
#    TARGETS debug
#    LD "-mlsp=${SdkRootDirPath}/examples/_boards/mimxrt700evk/dsp_examples/linker/hifi4/gdbio"
#)
#mcux_add_xtensa_configuration(
#    TARGETS release
#    LD "-mlsp=${SdkRootDirPath}/examples/_boards/mimxrt700evk/dsp_examples/linker/hifi4/min-rt"
#)
#
