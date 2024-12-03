cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(asrc_m2m_edma LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/asrc/asrc_m2m_edma)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/asrc/asrc_m2m_edma/asrc_m2m_edma.c
            examples/driver_examples/asrc/asrc_m2m_edma/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/asrc/asrc_m2m_edma
             examples/_boards/${board}/driver_examples/asrc/asrc_m2m_edma/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/asrc/asrc_m2m_edma/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)