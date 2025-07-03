cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(sram_emulator LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/sramc/sram_emulator)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/sramc/sram_emulator/sram_emulator.c
             examples/driver_examples/sramc/sram_emulator/flexio_sram_emulator.c
             examples/driver_examples/sramc/sram_emulator/flexio_sram_emulator.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/sramc/sram_emulator
             ${board_root}/${board}/driver_examples/sramc/sram_emulator/${core_id}
              examples/driver_examples/sramc/sram_emulator
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/sramc/sram_emulator/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)