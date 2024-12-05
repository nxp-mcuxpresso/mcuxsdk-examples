cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tsc_fourWireModeInterrupt LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/tsc/normal/fourWireMode/fourWireModeInterrupt)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/tsc/normal/fourWireMode/fourWireModeInterrupt/fourWireModeInterrupt.c
            ${board_root}/${board}/driver_examples/tsc/adc_config.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/tsc/normal/fourWireMode/fourWireModeInterrupt
             ${board_root}/${board}/driver_examples/tsc/normal/fourWireMode/fourWireModeInterrupt/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/tsc/normal/fourWireMode/fourWireModeInterrupt/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)