# Add additional configuration
mcux_add_macro(
  CC "DRTCESL_PQ_OFF=1"
  )

mcux_add_iar_configuration(
  LD "--diag_suppress=Lt069"
  )

mcux_add_armgcc_configuration(
  TARGETS debug
  CC "-O1"
  CX "-O1"
  )

# remove preprocessor appconfig - not available for fixed-point application
mcux_project_remove_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES
      examples/_boards/${board}/demo_apps/mc_pmsm/pmsm_enc/${multicore_foldername}/m1_pmsm_preprocessor.h
)