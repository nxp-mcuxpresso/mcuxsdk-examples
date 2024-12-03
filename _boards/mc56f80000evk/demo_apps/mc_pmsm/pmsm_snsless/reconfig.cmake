# Add additional configuration

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/_boards/${board}/demo_apps/mc_pmsm/pmsm_snsless/${multicore_foldername}/Flash_config.h
)

mcux_remove_configuration(
  TOOLCHAINS codewarrior
  TARGETS flash_ldm_lpm_debug flash_ldm_lpm_release
  CC "-globalsInLowerMemory"
)
