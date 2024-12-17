cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tee_fault_core0 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/tee_fault/core0)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/tee_fault/tee_fault_core0.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/tee_fault
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/tee_fault/core0/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
# Settings to link to secondary core project
mcux_add_iar_configuration(
    LD "--image_input=${APPLICATION_BINARY_DIR}/../tee_fault_core1/core1_image.bin,_core1_image,__core1_image,4 \
        --keep _core1_image"
)

mcux_add_include(
    TOOLCHAINS mdk armgcc
    BASE_PATH ${APPLICATION_BINARY_DIR}
    INCLUDES ../tee_fault_core1
)
