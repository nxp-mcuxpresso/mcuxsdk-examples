# Add additional configuration
mcux_add_macro(
  TOOLCHAINS codewarrior
  CC "-D_SWITCH_TO_FAST_MODE_AT_STARTUP=1"
  )

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
      examples/_boards/${board}/demo_apps/mc_pmsm/pmsm_enc/${multicore_foldername}/Flash_config.h
      examples/_boards/${board}/demo_apps/mc_pmsm/pmsm_enc/${multicore_foldername}/startup_clock_mode_config.h
)
