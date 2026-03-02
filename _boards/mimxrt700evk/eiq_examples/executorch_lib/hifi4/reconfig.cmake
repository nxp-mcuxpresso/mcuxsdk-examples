mcux_add_xtensa_configuration(
  CC "-DXOS_CLOCK_FREQ=237500000 -mlongcalls -std=c99"
  CX "-stdlib=libc++ \
    -mlongcalls \
    -std=c++17"
)

mcux_remove_configuration(
  CC "-std=gnu99"
  CX "-std=c++11"
)

mcux_add_macro(
  CC "-DPRINTF_ADVANCED_ENABLE=1\
      -DPRINTF_FLOAT_ENABLE=1"
  CX "-DPRINTF_ADVANCED_ENABLE=1\
      -DPRINTF_FLOAT_ENABLE=1"
)

# mcux_add_xcc_configuration(
#     CX "-Wno-sign-compare\
#         -Wno-format\
#         -Wno-shift-count-overflow\
#         -Wno-deprecated-declarations\
#         -Wno-int-in-bool-context"
# )

#mcux_add_xtensa_configuration(
#    CX "-Wno-sign-compare\
#        -Wno-format\
#        -Wno-shift-count-overflow\
#        -Wno-deprecated-declarations\
#        -Wno-int-in-bool-context"
#    CC "-Wno-sign-compare\
#        -Wno-format\
#        -Wno-shift-count-overflow\
#        -Wno-deprecated-declarations\
#        -Wno-int-in-bool-context"
#)
#

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
#    LD "-mlsp=${SdkRootDirPath}/${board_root}/mimxrt700evk/dsp_examples/linker/hifi4/gdbio"
#)
#mcux_add_xtensa_configuration(
#    TARGETS release
#    LD "-mlsp=${SdkRootDirPath}/${board_root}/mimxrt700evk/dsp_examples/linker/hifi4/min-rt"
#)
#
