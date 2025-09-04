cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(asrc_m2m_interrupt LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/asrc/asrc_m2m_interrupt)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/asrc/asrc_m2m_interrupt/asrc_m2m_interrupt.c
            examples/driver_examples/asrc/asrc_m2m_interrupt/music.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/asrc/asrc_m2m_interrupt
             ${board_root}/${board}/driver_examples/asrc/asrc_m2m_interrupt/${core_id}
	     examples/driver_examples/asrc
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/asrc/asrc_m2m_interrupt/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)