cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(adc_etc_edma LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/adc_etc/adc_etc_edma)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/adc_etc/adc_etc_edma/adc_etc_edma.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/adc_etc/adc_etc_edma
             examples/_boards/${board}/driver_examples/adc_etc/adc_etc_edma/${core_id}
)

mcux_add_macro(
    CC "-DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1"
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/adc_etc/adc_etc_edma/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)