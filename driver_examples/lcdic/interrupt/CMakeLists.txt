cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lcdic_interrupt LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/lcdic/interrupt)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lcdic/interrupt/lcdic_interrupt.c
            examples/_boards/${board}/driver_examples/lcdic/lcdic_panel_config.h
            examples/driver_examples/lcdic/panel_func.c
            examples/driver_examples/lcdic/panel_func.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/lcdic/interrupt
             examples/driver_examples/lcdic
             examples/_boards/${board}/driver_examples/lcdic
             examples/_boards/${board}/driver_examples/lcdic/interrupt/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/lcdic/interrupt/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)