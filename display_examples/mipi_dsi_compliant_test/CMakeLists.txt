cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mipi_dsi_compliant_test LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/display_examples/mipi_dsi_compliant_test)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/display_examples/mipi_dsi_compliant_test/mipi_dsi_compliant_test.c
            examples/_boards/${board}/display_support.h
            examples/_boards/${board}/display_support.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}
             examples/_boards/${board}/display_examples/mipi_dsi_compliant_test
             examples/_boards/${board}/display_examples/mipi_dsi_compliant_test/${multicore_foldername}
)

include(${SdkRootDirPath}/examples/_boards/${board}/display_examples/mipi_dsi_compliant_test/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)