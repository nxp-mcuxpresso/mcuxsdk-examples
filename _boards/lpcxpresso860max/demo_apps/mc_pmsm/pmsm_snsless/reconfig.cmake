# Add additional configuration
mcux_add_macro(
  CC "CPU_LPC865M201JBD64"
  )

  mcux_add_configuration(
  TARGETS debug
  TOOLCHAINS armgcc mcux
  CC "-O3"
  CX "-O3"
  )
