# Add additional configuration

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES ${board_root}/${board}/demo_apps/mc_pmsm/pmsm_enc/${multicore_foldername}/Flash_config.h
)
