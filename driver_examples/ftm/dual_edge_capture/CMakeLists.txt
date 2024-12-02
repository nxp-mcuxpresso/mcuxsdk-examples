cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(ftm_dual_edge_capture LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/ftm/dual_edge_capture)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/ftm/dual_edge_capture/ftm_dual_edge_capture.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/ftm/dual_edge_capture
             examples/_boards/${board}/driver_examples/ftm/dual_edge_capture/${core_id}
)

mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=1"
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/ftm/dual_edge_capture/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)