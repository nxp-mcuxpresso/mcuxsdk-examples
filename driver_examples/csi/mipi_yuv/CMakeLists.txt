cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(csi_mipi_yuv LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/csi/mipi_yuv)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/csi/mipi_yuv/csi_mipi_yuv.c
            examples/_boards/${board}/display_support.h
            examples/_boards/${board}/display_support.c
            examples/_boards/${board}/camera_support.h
            examples/_boards/${board}/camera_support.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/csi/mipi_yuv
             examples/_boards/${board}
             examples/_boards/${board}/driver_examples/csi/mipi_yuv/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/csi/mipi_yuv/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)