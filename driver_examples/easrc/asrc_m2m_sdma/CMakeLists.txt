cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(easrc_m2m_sdma LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/asrc/asrc_m2m_sdma)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/easrc/asrc_m2m_sdma/asrc_m2m_sdma.c
            examples/driver_examples/easrc/asrc_m2m_sdma/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/asrc/asrc_m2m_sdma
             examples/_boards/${board}/driver_examples/asrc/asrc_m2m_sdma/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/asrc/asrc_m2m_sdma/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)