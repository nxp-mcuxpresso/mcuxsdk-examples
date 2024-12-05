
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(soem_gpio_pulse_netc_freertos LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/soem_examples/soem_gpio_pulse/freertos)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/soem_examples/soem_gpio_pulse_netc/freertos/soem_gpio_pulse.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/soem_examples/soem_gpio_pulse_netc/freertos
)

include(${SdkRootDirPath}/${board_root}/${board}/soem_examples/soem_gpio_pulse/freertos/${core_id}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)