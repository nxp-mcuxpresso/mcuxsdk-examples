cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(trdc_flw LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/trdc/flw)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/trdc/flw/trdc_flw.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/trdc/flw/${core_id}
             examples/_boards/${board}/driver_examples/trdc/${core_id}
)

mcux_add_macro(
    CC "-D__SEMIHOST_HARDFAULT_DISABLE=1"
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/trdc/flw/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/core1_image.bin)