cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(igpio_input_eqdc_signal LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/driver_examples/gpio/input_eqdc_signal)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/igpio/input_eqdc_interrupt/gpio_input_eqdc_signal.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/driver_examples/gpio/input_eqdc_signal
             examples/_boards/${board}/driver_examples/gpio/input_eqdc_signal/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/driver_examples/gpio/input_eqdc_signal/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)