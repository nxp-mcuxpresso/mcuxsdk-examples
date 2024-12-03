
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(isi_dwc_mipi_csi2_rgb565 LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/isi/dwc_mipi_csi2/rgb565)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_include(
    INCLUDES .
)

mcux_add_source(
    SOURCES isi_dwc_mipi_csi2_rgb565.c
)

mcux_add_source(
    SOURCES ../app_display.c
            ../app_display.h
)

mcux_add_include(
    INCLUDES ../
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/isi/dwc_mipi_csi2/rgb5565/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
