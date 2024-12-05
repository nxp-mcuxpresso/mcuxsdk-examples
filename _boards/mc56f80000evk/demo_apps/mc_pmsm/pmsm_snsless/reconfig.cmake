# Add additional configuration

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES ${board_root}/${board}/demo_apps/mc_pmsm/pmsm_enc/${multicore_foldername}/Flash_config.h
)

mcux_remove_configuration(
  TOOLCHAINS codewarrior
  TARGETS flash_ldm_lpm_debug flash_ldm_lpm_release
  CC "-globalsInLowerMemory"
)
