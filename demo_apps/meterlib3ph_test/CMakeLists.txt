cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(meterlib3ph_test LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/demo_apps/meterlib3ph_test)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_macro(
  CC "SDK_DEBUGCONSOLE_UART"
  )

mcux_add_configuration(
    TOOLCHAINS armgcc mdk
    CC "-ffreestanding"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        ${board_root}/${board}/demo_apps/meterlib3ph_test/${multicore_foldername}/meterlib3ph_test.c
        ${board_root}/${board}/demo_apps/meterlib3ph_test/${multicore_foldername}/meterlib3ph_cfg.h
        ${board_root}/${board}/demo_apps/meterlib3ph_test/${multicore_foldername}/board.c
        ${board_root}/${board}/demo_apps/meterlib3ph_test/${multicore_foldername}/board.h
        ${board_root}/${board}/demo_apps/meterlib3ph_test/${multicore_foldername}/clock_config.c
        ${board_root}/${board}/demo_apps/meterlib3ph_test/${multicore_foldername}/clock_config.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/meterlib3ph_test/${multicore_foldername}
)

include(${SdkRootDirPath}/${board_root}/${board}/demo_apps/meterlib3ph_test/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
