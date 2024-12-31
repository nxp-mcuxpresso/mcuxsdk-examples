cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(endat3_point2point LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/digital_encoder/endat3)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/digital_encoder/endat3/endat3_point2point/endat3.c
)

mcux_add_macro(
    CC "-DENDAT3_PARTICIPANTS_NUM=1"
)

include(${SdkRootDirPath}/${board_root}/${board}/digital_encoder/endat3/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
