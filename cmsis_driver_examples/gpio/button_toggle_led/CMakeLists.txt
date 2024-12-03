# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(cmsis_button_toggle_led LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/cmsis_driver_examples/gpio/button_toggle_led)

include(${SdkRootDirPath}/CMakeLists.txt)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/cmsis_driver_examples/gpio/button_toggle_led/cmsis_button_toggle_led.c
            examples/_boards/${board}/cmsis_driver_examples/gpio/button_toggle_led/RTE_Device.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/cmsis_driver_examples/gpio/button_toggle_led
             examples/_boards/${board}/cmsis_driver_examples/gpio/button_toggle_led/${core_id}
)

include(${SdkRootDirPath}/examples/_boards/${board}/cmsis_driver_examples/gpio/button_toggle_led/reconfig.cmake OPTIONAL)

mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)