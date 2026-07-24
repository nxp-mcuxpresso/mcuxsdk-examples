cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(power_mode_switch_rt2660 LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/power_mode_switch)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_configuration(
    CC  "-DPRINTF_ADVANCED_ENABLE=1"
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/examples
    SOURCES demo_apps/power_mode_switch_rt2660/power_mode_switch.c
            demo_apps/power_mode_switch_rt2660/power_mode_switch.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/examples
    INCLUDES demo_apps/power_mode_switch_rt2660
             _boards/${board}/demo_apps/power_mode_switch
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/power_mode_switch/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
