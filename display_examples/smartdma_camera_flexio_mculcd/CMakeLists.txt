cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(smartdma_camera_flexio_mculcd LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/display_examples/smartdma_camera_flexio_mculcd)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/display_examples/smartdma_camera_flexio_mculcd/smartdma_camera_flexio_mculcd.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/display_examples/smartdma_camera_flexio_mculcd
             examples/_boards/${board}/display_examples/smartdma_camera_flexio_mculcd/${multicore_foldername}
)

include(${SdkRootDirPath}/examples/_boards/${board}/display_examples/smartdma_camera_flexio_mculcd/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)