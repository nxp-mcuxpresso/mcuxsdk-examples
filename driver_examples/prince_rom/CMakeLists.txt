cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(prince_rom LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/prince_rom)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/prince/prince_rom.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/prince_rom
             ${board_root}/${board}/driver_examples/prince_rom/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/prince_rom/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)

mcux_add_linker_symbol(
    SYMBOLS "__stack_size__=0x2000"
)