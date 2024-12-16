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

# Remove default linker file - SDM configurations
mcux_remove_codewarrior_linker_script(
  TARGETS flash_sdm_lpm_debug flash_sdm_lpm_release
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/${soc_portfolio}/${soc_series}/${device}/codewarrior/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_Internal_PFlash_SDM.cmd
)

# Use linker file updated for motor control application - SDM configurations
mcux_add_codewarrior_linker_script(
  TARGETS flash_sdm_lpm_debug flash_sdm_lpm_release
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/demo_apps/mc_pmsm/pmsm_enc/${multicore_foldername}/mc_linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_Internal_PFlash_SDM_mc_pmsm.cmd
)

# Remove default linker file - LDM configurations
mcux_remove_codewarrior_linker_script(
  TARGETS flash_ldm_lpm_debug flash_ldm_lpm_release
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/${soc_portfolio}/${soc_series}/${device}/codewarrior/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_Internal_PFlash_LDM.cmd
)

# Use linker file updated for motor control application - LDM configurations
mcux_add_codewarrior_linker_script(
  TARGETS flash_ldm_lpm_debug flash_ldm_lpm_release
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/${board}/demo_apps/mc_pmsm/pmsm_enc/${multicore_foldername}/mc_linker/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_Internal_PFlash_LDM_mc_pmsm.cmd
)