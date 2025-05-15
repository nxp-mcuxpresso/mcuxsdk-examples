# Add additional configuration
mcux_project_remove_source(
  SOURCES
	clock_config.h
	clock_config.c
)


# remove MC SINC driver, EnDat2.2 is used for single motor control application
mcux_project_remove_source(
  BASE_PATH ${SdkRootDirPath}/middleware/motor_control
  SOURCES pmsm/pmsm_float/mc_drivers/mcdrv_bissc.c
		  pmsm/pmsm_float/mc_drivers/mcdrv_bissc.h
)
