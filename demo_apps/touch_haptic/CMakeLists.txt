cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(touch_haptic LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/demo_apps/touch_haptic)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_macro(
  CC "-DSDK_DEBUGCONSOLE_UART -DFSL_FEATURE_TSI_S_XDN_HAS_THREE_VALID_BITS"
  )

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/freemaster_cfg.h
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/main.c
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/main.h
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/nt_setup.c
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/nt_setup.h
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/board.c
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/board.h
		${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/pin_mux.c
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/pin_mux.h
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/fix_lib.c
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/fix_lib.h
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/fix_types.h
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/haptic.c
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/haptic.h
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/haptic_pwm.c
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/haptic_pwm.h
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/ui_buttons.c
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/ui_buttons.h
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/ui_control.c
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/ui_control.h
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/wav/audiohaptic_sigs.h
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/wav/audiohaptic_sigs_8khz.h
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/wav/audiohaptic1.wav.h
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/wav/audiohaptic2.wav.h
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/wav/intro_wav.h
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/wav/superstition_wav.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}
        ${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/wav
)

include(${SdkRootDirPath}/${board_root}/${board}/demo_apps/touch_haptic/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
