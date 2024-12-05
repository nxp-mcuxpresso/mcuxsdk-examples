cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mcx_opamp_dac_adc LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/opamp/opamp_dac_adc)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/opamp/opamp_dac_adc/opamp_dac_adc.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/opamp/opamp_dac_adc
             ${board_root}/${board}/driver_examples/opamp/opamp_dac_adc/${core_id}
)

mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=1"
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/opamp/opamp_dac_adc/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)