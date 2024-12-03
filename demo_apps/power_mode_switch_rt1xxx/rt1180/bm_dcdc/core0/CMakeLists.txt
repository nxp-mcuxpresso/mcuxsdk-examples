cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(power_mode_switch_bm_dcdc_rt1xxx_core0 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/power_mode_switch/bm_dcdc/core0)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES 
        examples/demo_apps/power_mode_switch_rt1xxx/rt1180/power_mode_switch_core0.c
        examples/demo_apps/power_mode_switch_rt1xxx/rt1180/lpm.c
        examples/demo_apps/power_mode_switch_rt1xxx/rt1180/lpm.h
        examples/demo_apps/power_mode_switch_rt1xxx/rt1180/chip_init_def.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/power_mode_switch_rt1xxx/rt1180
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/power_mode_switch/bm_dcdc/core0/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

# Settings to link to secondary core project
mcux_add_iar_configuration(
    LD "--image_input=${APPLICATION_BINARY_DIR}/../power_mode_switch_bm_dcdc_rt1xxx_core1/iar/core1_image.bin,_power_mode_switch_bm_dcdc_core1_image,__core1_image,4 \
    --keep _power_mode_switch_bm_dcdc_core1_image"
)

mcux_add_mdk_configuration(
    LD "--keep=*(*core1_code)"
)
mcux_add_macro(
    CC "-DMCMGR_HANDLE_EXCEPTIONS=1\
       -D__SEMIHOST_HARDFAULT_DISABLE=1"
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../power_mode_switch_bm_dcdc_rt1xxx_core1/${CONFIG_TOOLCHAIN}
)
