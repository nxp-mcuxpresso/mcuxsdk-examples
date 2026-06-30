cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(power_mode_switch_bm_dcdc_rt1xxx LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/demo_apps/power_mode_switch/bm_dcdc)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES 
        examples/demo_apps/power_mode_switch_rt1xxx/rt1150/power_mode_switch.c
        examples/demo_apps/power_mode_switch_rt1xxx/rt1150/lpm.c
        examples/demo_apps/power_mode_switch_rt1xxx/rt1150/lpm.h
        examples/demo_apps/power_mode_switch_rt1xxx/rt1150/setpoint_table_def.h
        examples/demo_apps/power_mode_switch_rt1xxx/rt1150/chip_init_def.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/power_mode_switch_rt1xxx/rt1150
)

include(${SdkRootDirPath}/${board_root}/${board}/demo_apps/power_mode_switch/bm_dcdc/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

mcux_add_configuration(
    TOOLCHAINS armgcc iar mdk
    CC "-DSINGLE_CORE_M7"
)

mcux_add_configuration(
    TOOLCHAINS armgcc iar mdk
    TARGETS flexspi_nor_debug flexspi_nor_release
    CC "-DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1"
)

mcux_add_macro(
    TOOLCHAINS armgcc
    TARGETS flexspi_nor_debug flexspi_nor_release
    AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
)