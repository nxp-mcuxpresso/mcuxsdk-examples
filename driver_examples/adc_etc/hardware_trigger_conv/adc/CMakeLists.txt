cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(adc_etc_hardware_trigger_conv LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/adc_etc/adc_etc_hardware_trigger_conv)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    SOURCES ./adc_etc_hardware_trigger_conv.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/adc_etc/adc_etc_hardware_trigger_conv
             ${board_root}/${board}/driver_examples/adc_etc/adc_etc_hardware_trigger_conv/${core_id}
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/adc_etc/adc_etc_hardware_trigger_conv/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
