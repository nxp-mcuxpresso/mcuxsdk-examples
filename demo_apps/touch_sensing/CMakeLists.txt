cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(touch_sensing LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/demo_apps/touch_sensing)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_macro(
  CC "SDK_DEBUGCONSOLE_UART"
  )

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        ${board_root}/${board}/demo_apps/touch_sensing/${multicore_foldername}/freemaster_cfg.h
        ${board_root}/${board}/demo_apps/touch_sensing/${multicore_foldername}/main.c
        ${board_root}/${board}/demo_apps/touch_sensing/${multicore_foldername}/main.h
        ${board_root}/${board}/demo_apps/touch_sensing/${multicore_foldername}/nt_setup.c
        ${board_root}/${board}/demo_apps/touch_sensing/${multicore_foldername}/nt_setup.h
        ${board_root}/${board}/demo_apps/touch_sensing/${multicore_foldername}/board.c
        ${board_root}/${board}/demo_apps/touch_sensing/${multicore_foldername}/board.h
        ${board_root}/${board}/demo_apps/touch_sensing/${multicore_foldername}/pin_mux.c
        ${board_root}/${board}/demo_apps/touch_sensing/${multicore_foldername}/pin_mux.h

)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/touch_sensing/${multicore_foldername}
)

include(${SdkRootDirPath}/${board_root}/${board}/demo_apps/touch_sensing/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
