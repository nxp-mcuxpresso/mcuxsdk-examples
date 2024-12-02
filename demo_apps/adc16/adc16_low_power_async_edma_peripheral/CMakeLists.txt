cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(adc16_low_power_async_edma_peripheral LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/demo_apps/adc16_low_power_async_edma_peripheral)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/adc16/adc16_low_power_async_edma_peripheral/adc16_low_power_async_edma_peripheral.c
)

include(${SdkRootDirPath}/examples/_boards/${board}/demo_apps/adc16_low_power_async_edma_peripheral/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)