# Add additional configuration
mcux_add_macro(
  CC "-DRTCESL_PQ_ON=1"
  )

mcux_add_iar_configuration(
  TARGETS release
  CC "--no_unaligned_access"
  )

mcux_add_armgcc_configuration(
  CC "-Wno-uninitialized"
  )

  mcux_add_iar_configuration(
      LD "--config_def=__stack_size__=0x2000\
          --config_def=__heap_size__=0x1000"
  )
