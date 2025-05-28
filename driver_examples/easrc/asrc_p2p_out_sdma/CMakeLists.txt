cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(easrc_p2p_out_sdma LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/asrc/asrc_p2p_out_sdma)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/easrc/asrc_p2p_out_sdma/asrc_p2p_out_sdma.c
            examples/driver_examples/easrc/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/asrc/asrc_p2p_out_sdma
             ${board_root}/${board}/driver_examples/asrc/asrc_p2p_out_sdma/${core_id}
	     examples/driver_examples/easrc
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/asrc/asrc_p2p_out_sdma/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)