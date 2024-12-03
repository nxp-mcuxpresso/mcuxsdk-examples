cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(connectivity_test_freertos LANGUAGES C CXX ASM 
    PROJECT_BOARD_PORT_PATH examples/_boards/${board}/wireless_examples
)

include(../CMakeLists.txt)

mcux_add_configuration(
    CC "        -DSDK_OS_FREE_RTOS"
)
