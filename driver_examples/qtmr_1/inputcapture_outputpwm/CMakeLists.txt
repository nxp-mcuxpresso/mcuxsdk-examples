cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(qtmr_inputcapture_outputpwm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/qtmr/inputcapture_outputpwm)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/qtmr_1/inputcapture_outputpwm/qtmr_inputcapture_outputpwm.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/qtmr/inputcapture_outputpwm
             examples/_boards/${board}/driver_examples/qtmr/inputcapture_outputpwm/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/qtmr/inputcapture_outputpwm/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)