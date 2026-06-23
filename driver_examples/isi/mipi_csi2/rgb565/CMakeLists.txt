cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(isi_mipi_csi2_rgb565
    LANGUAGES C CXX ASM
    PROJECT_BOARD_PORT_PATH examples_int/_boards/${board}/driver_examples/isi/mipi_csi2/rgb565
)

mcux_add_include(
    INCLUDES .
)

mcux_add_source(
    SOURCES isi_mipi_csi2_rgb565.c
)

include(${SdkRootDirPath}/CMakeLists.txt)

include(${SdkRootDirPath}/examples_int/_boards/${board}/driver_examples/isi/mipi_csi2/rgb565/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
