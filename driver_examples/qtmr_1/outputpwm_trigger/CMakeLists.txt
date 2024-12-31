cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(qtmr_outputpwm_trigger LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/qtmr/outputpwm_trigger)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/qtmr_1/outputpwm_trigger/qtmr_outputpwm.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/qtmr/outputpwm_trigger
             ${board_root}/${board}/driver_examples/qtmr/outputpwm_trigger/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/qtmr/outputpwm_trigger/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
