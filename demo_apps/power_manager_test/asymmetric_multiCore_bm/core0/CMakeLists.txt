cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(power_manager_test_asymmetric_multiCore_bm_core0 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/power_manager_test/multiCore_bm/core0)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/power_manager_test/asymmetric_multiCore_bm/power_manager_test_multiCore_bm_core0.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/power_manager_test/asymmetric_multiCore_bm
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/power_manager_test/multiCore_bm/core0/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
# Settings to link to secondary core project
mcux_add_iar_configuration(
    LD "--image_input=${APPLICATION_BINARY_DIR}/../power_manager_test_asymmetric_multiCore_bm_core1/iar/core1_image.bin,_power_manager_test_asymmetric_multiCore_bm_core1_image,__core1_image,4 \
    --keep _power_manager_test_asymmetric_multiCore_bm_core1_image"
)

mcux_add_mdk_configuration(
    LD "--keep=*(*core1_code)"
)

mcux_add_macro(
    CC "-DPM_PM_DUAL_CORE_USED=1"
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../power_manager_test_asymmetric_multiCore_bm_core1/${CONFIG_TOOLCHAIN}
)
