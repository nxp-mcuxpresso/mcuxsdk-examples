mcux_remove_iar_linker_script(
  BASE_PATH
  ${SdkRootDirPath}
  TARGETS
  debug
  release
  LINKER
  ${device_root}/MCX/MCXL/MCXL255/iar/MCXL255_cm33_flash.icf)

mcux_add_iar_linker_script(
  BASE_PATH
  ${SdkRootDirPath}
  TARGETS
  debug
  release
  LINKER
  ${board_root}/${board}/demo_apps/aon_lpuart_wakeup/cm33/linkers/power_cm33_flash.icf
)

mcux_remove_armgcc_linker_script(
  BASE_PATH
  ${SdkRootDirPath}
  TARGETS
  debug
  release
  LINKER
  ${device_root}/MCX/MCXL/MCXL255/gcc/MCXL255_cm33_flash.ld)

mcux_add_armgcc_linker_script(
  BASE_PATH
  ${SdkRootDirPath}
  TARGETS
  debug
  release
  LINKER
  ${board_root}/${board}/demo_apps/aon_lpuart_wakeup/cm33/linkers/power_cm33_flash.ld
)
