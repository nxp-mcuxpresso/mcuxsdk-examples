cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(power_manager_test_asymmetric_multiCore_bm_core1 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/power_manager_test/multiCore_bm/core1)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/power_manager_test/asymmetric_multiCore_bm/power_manager_test_multiCore_bm_core1.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/power_manager_test/asymmetric_multiCore_bm
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/power_manager_test/multiCore_bm/core1/reconfig.cmake OPTIONAL)

mcux_convert_binary(
    TOOLCHAINS armgcc mdk iar
    BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/core1_image.bin
)

mcux_add_macro(
    CC "-DPM_PM_DUAL_CORE_USED=1"
)
