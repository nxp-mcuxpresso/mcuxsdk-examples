cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(dsc_dac_pit_sync_sawtooth_waveform LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/dac_external_sync/pit_sync_sawtooth_waveform)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/dsc_dac/dsc_dac_external_sync/pit_sync_sawtooth_waveform/dac_pit_sync_sawtooth_waveform.c
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/dac_external_sync/pit_sync_sawtooth_waveform/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)