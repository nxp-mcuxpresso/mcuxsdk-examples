cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(csi_mipi_rgb LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/csi/mipi_rgb)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/csi/mipi_rgb/csi_mipi_rgb.c
            ${board_root}/${board}/camera_support.h
            ${board_root}/${board}/camera_support.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/csi/mipi_rgb
             ${board_root}/${board}/driver_examples/csi/mipi_rgb/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/csi/mipi_rgb/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)