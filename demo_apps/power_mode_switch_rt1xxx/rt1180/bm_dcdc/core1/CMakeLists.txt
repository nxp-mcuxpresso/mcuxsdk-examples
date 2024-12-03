cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(power_mode_switch_bm_dcdc_rt1xxx_core1 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/power_mode_switch/bm_dcdc/core1)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/demo_apps/power_mode_switch_rt1xxx/rt1180/power_mode_switch_core1.c
        examples/demo_apps/power_mode_switch_rt1xxx/rt1180/lpm.c
        examples/demo_apps/power_mode_switch_rt1xxx/rt1180/lpm.h
        examples/demo_apps/power_mode_switch_rt1xxx/rt1180/chip_init_def.h
    
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/power_mode_switch_rt1xxx/rt1180
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/power_mode_switch/bm_dcdc/core1/reconfig.cmake OPTIONAL)

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/core1_image.bin
)
