cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mc_bldc LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/demo_apps/mc_bldc)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_macro(
  CC "MC_EXAMPLE"
  )

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        ${board_root}/${board}/demo_apps/mc_bldc/${multicore_foldername}/main.c
        ${board_root}/${board}/demo_apps/mc_bldc/${multicore_foldername}/m1_bldc_appconfig.h
        ${board_root}/${board}/demo_apps/mc_bldc/${multicore_foldername}/clock_config.c
        ${board_root}/${board}/demo_apps/mc_bldc/${multicore_foldername}/clock_config.h
        ${board_root}/${board}/demo_apps/mc_bldc/${multicore_foldername}/pin_mux.c
        ${board_root}/${board}/demo_apps/mc_bldc/${multicore_foldername}/pin_mux.h
        ${board_root}/${board}/demo_apps/mc_bldc/${multicore_foldername}/peripherals.c
        ${board_root}/${board}/demo_apps/mc_bldc/${multicore_foldername}/peripherals.h
        ${board_root}/${board}/demo_apps/mc_bldc/${multicore_foldername}/freemaster_cfg.h
        ${board_root}/${board}/demo_apps/mc_bldc/${multicore_foldername}/mc_periph_init.c
        ${board_root}/${board}/demo_apps/mc_bldc/${multicore_foldername}/mc_periph_init.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/mc_bldc/${multicore_foldername}
)

mcux_add_iar_configuration(
    CX "--diag_suppress=Pa082,Pa050"
)

include(${SdkRootDirPath}/${board_root}/${board}/demo_apps/mc_bldc/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
