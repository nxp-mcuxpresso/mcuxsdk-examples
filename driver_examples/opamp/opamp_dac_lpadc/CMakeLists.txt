cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(mcx_opamp_dac_lpadc LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/opamp/opamp_dac_lpadc)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/opamp/opamp_dac_lpadc/opamp_dac_lpadc.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/opamp/opamp_dac_lpadc
             examples/_boards/${board}/driver_examples/opamp/opamp_dac_lpadc/${core_id}
)

mcux_add_macro(
    CC "-DPRINTF_FLOAT_ENABLE=1"
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/opamp/opamp_dac_lpadc/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)