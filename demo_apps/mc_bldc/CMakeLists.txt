cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mc_bldc LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/mc_bldc)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_macro(
  CC "MC_EXAMPLE"
  )

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/_boards/${board}/demo_apps/mc_bldc/${multicore_foldername}/main.c
        examples/_boards/${board}/demo_apps/mc_bldc/${multicore_foldername}/m1_bldc_appconfig.h
        examples/_boards/${board}/demo_apps/mc_bldc/${multicore_foldername}/clock_config.c
        examples/_boards/${board}/demo_apps/mc_bldc/${multicore_foldername}/clock_config.h
        examples/_boards/${board}/demo_apps/mc_bldc/${multicore_foldername}/pin_mux.c
        examples/_boards/${board}/demo_apps/mc_bldc/${multicore_foldername}/pin_mux.h
        examples/_boards/${board}/demo_apps/mc_bldc/${multicore_foldername}/peripherals.c
        examples/_boards/${board}/demo_apps/mc_bldc/${multicore_foldername}/peripherals.h
        examples/_boards/${board}/demo_apps/mc_bldc/${multicore_foldername}/freemaster_cfg.h
        examples/_boards/${board}/demo_apps/mc_bldc/${multicore_foldername}/mc_periph_init.c
        examples/_boards/${board}/demo_apps/mc_bldc/${multicore_foldername}/mc_periph_init.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/demo_apps/mc_bldc/${multicore_foldername}
)

mcux_add_iar_configuration(
    CX "--diag_suppress=Pa082,Pa050"
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/mc_bldc/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
