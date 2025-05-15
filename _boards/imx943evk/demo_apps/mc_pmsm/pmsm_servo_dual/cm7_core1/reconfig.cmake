# Add additional configuration
mcux_project_remove_source(
  SOURCES
	clock_config.h
	clock_config.c
)

mcux_add_macro(
  CC "-DSERVO_OPTIM"
  )