cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(lpadc_temperature_measurement LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/driver_examples/lpadc/temperature_measurement)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/lpadc/temperature_measurement/lpadc_temperature_measurement.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/lpadc/temperature_measurement
             ${board_root}/${board}/driver_examples/lpadc/temperature_measurement/${core_id}
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1"
)

include(${SdkRootDirPath}/${board_root}/${board}/driver_examples/lpadc/temperature_measurement/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)