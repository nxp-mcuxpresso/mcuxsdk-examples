cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mmccard_boot_non_blocking LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/mmccard/boot/non_blocking)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/mmccard/boot/non_blocking/mmccard_boot.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/mmccard/boot/non_blocking
             examples/_boards/${board}/driver_examples/mmccard/boot/non_blocking/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/mmccard/boot/non_blocking/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)