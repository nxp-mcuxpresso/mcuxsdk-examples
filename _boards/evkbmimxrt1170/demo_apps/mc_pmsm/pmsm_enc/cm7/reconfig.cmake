# Add additional configuration

mcux_add_mdk_configuration(
  LD "--diag_suppress=6329"
)

mcux_add_configuration(
  TOOLCHAINS mcux
  LD "-Xlinker --defsym=__stack_size__=0x1000\
      -Xlinker --defsym=__heap_size__=0x1000"
)
