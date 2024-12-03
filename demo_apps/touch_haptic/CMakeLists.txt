cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(touch_haptic LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/touch_haptic)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_macro(
  CC "-DSDK_DEBUGCONSOLE_UART -DFSL_FEATURE_TSI_S_XDN_HAS_THREE_VALID_BITS"
  )

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/freemaster_cfg.h
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/main.c
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/main.h
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/nt_setup.c
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/nt_setup.h
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/board.c
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/board.h
		examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/pin_mux.c
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/pin_mux.h
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/fix_lib.c
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/fix_lib.h
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/fix_types.h
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/haptic.c
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/haptic.h
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/haptic_pwm.c
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/haptic_pwm.h
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/ui_buttons.c
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/ui_buttons.h
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/ui_control.c
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/ui_control.h
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/wav/audiohaptic_sigs.h
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/wav/audiohaptic_sigs_8khz.h
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/wav/audiohaptic1.wav.h
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/wav/audiohaptic2.wav.h
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/wav/intro_wav.h
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/wav/superstition_wav.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}
        examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/wav
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/touch_haptic/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
