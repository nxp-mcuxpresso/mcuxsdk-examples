cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(csi_fragment_gray LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/csi/fragment_gray)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/csi/fragment_gray/csi_fragment_gray.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/csi/fragment_gray
             examples/_boards/${board}/driver_examples/csi/fragment_gray/${core_id}
)

mcux_add_macro(
    CC "-DCSI_DRIVER_FRAG_MODE=1"
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/csi/fragment_gray/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)