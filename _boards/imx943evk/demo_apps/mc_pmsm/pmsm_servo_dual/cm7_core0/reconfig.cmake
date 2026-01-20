# Add additional configuration
mcux_project_remove_source(
  SOURCES
	clock_config.h
	clock_config.c
)

mcux_add_macro(
  CC "-DSERVO_OPTIM"
  )
  
# remove MC BiSS and EnDat3 drivers added by MCUX_COMPONENT_middleware.motor_control.mc_drivers_imx943evk component
# EnDat2.2 is used for servo dual motor control application
mcux_project_remove_source(
  BASE_PATH ${SdkRootDirPath}/middleware/motor_control
  SOURCES pmsm/pmsm_float/mc_drivers/mcdrv_bissc.c
		  pmsm/pmsm_float/mc_drivers/mcdrv_bissc.h
		  pmsm/pmsm_float/mc_drivers/mcdrv_endat3.c
		  pmsm/pmsm_float/mc_drivers/mcdrv_endat3.h
)