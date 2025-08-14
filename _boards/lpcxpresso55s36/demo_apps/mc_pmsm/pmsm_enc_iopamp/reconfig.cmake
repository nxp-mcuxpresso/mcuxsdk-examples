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

# Remove MC flash driver - not available in pmsm_enc_iopamp motor application
mcux_project_remove_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES middleware/motor_control/pmsm/pmsm_float/mc_drivers/mcdrv_flash_lpc55s36.h
          middleware/motor_control/pmsm/pmsm_float/mc_drivers/mcdrv_flash_lpc55s36.c
)

# Remove MC flash driver macro - not available in pmsm_enc_iopamp motor application
mcux_remove_macro(
  CC "-DMCDRV_FLASH_LPC55S36"
)