
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(soem_gpio_pulse_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/soem_examples/soem_gpio_pulse/freertos)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/soem_examples/soem_gpio_pulse/freertos/soem_gpio_pulse.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/soem_examples/soem_gpio_pulse/freertos
)

mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__stack_size__=0x2000 -Xlinker --defsym=__heap_size__=0x10000"
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x2000 --config_def=__heap_size__=0x10000"
)

mcux_add_mdk_configuration(
    LD "--predefine=\"-D__stack_size__=0x2000\" --predefine=\"-D__heap_size__=0x10000\""
)

include(${SdkRootDirPath}/examples/_boards/${board}/soem_examples/soem_gpio_pulse/freertos/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)