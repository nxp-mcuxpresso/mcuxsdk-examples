cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dsc_dac_ext_triggered_waveform LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/dac/ext_triggered_waveform)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/dsc_dac/ext_triggered_waveform/dac_ext_triggered_waveform.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/dac/ext_triggered_waveform
             examples/_boards/${board}/driver_examples/dac/ext_triggered_waveform/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/dac/ext_triggered_waveform/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)