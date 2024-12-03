
cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(soem_gpio_pulse_bm LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/soem_examples/soem_gpio_pulse/bm)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/soem_examples/soem_gpio_pulse/bm/soem_gpio_pulse.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/soem_examples/soem_gpio_pulse/bm
)

include(${SdkRootDirPath}/examples/_boards/${board}/soem_examples/soem_gpio_pulse/bm/${multicore_foldername}/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)