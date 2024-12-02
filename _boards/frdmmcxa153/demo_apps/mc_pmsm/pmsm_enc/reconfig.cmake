# Add additional configuration
mcux_add_macro(
  CC "DRTCESL_PQ_OFF=1"
  )

mcux_add_iar_configuration(
  LD "--diag_suppress=Lt069"
  )

mcux_add_armgcc_configuration(
  TARGETS debug
  CC "-O1"
  CX "-O1"
  )
