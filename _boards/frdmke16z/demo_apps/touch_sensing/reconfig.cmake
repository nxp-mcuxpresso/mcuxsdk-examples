# Add additional configuration
mcux_add_macro(
  CC "NT_SAFETY_SUPPORT=0"
  )

mcux_add_configuration(
  TOOLCHAINS mcux
  TARGETS debug
  CC "-O1"
)