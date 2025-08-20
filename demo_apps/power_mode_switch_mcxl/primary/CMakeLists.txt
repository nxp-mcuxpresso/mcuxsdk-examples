cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(power_mode_switch_mcxl_primary LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/demo_apps/power_mode_switch)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_iar_configuration(
    LD "--image_input=${APPLICATION_BINARY_DIR}/../power_mode_switch_mcxl_secondary/iar/core1_image.bin,_core1_image,__core1_image,4 \
        --keep=_core1_image"
)

mcux_add_mdk_configuration(
    LD "--library_type=standardlib --keep=*(*core1_code)"
)

mcux_add_configuration(
    TOOLCHAINS armgcc iar mdk
    CC "-DCORE1_IMAGE_COPY_TO_RAM\
       -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1"
    AS "-D__ENABLE_LP_BOOT=1"
)

mcux_add_linker_symbol(
    SYMBOLS __use_shmem__=1
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/power_mode_switch_mcxl/primary/power_mode_switch_primary.c
            examples/demo_apps/power_mode_switch_mcxl/primary/power_mode_switch_primary.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/power_mode_switch_mcxl/primary
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../power_mode_switch_mcxl_secondary/${CONFIG_TOOLCHAIN}/
)

include(${SdkRootDirPath}/${board_root}/${board}/demo_apps/power_mode_switch/cm33/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
