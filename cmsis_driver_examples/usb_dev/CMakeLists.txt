# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  usb_device_cmsis_validation
  LANGUAGES
    "ASM"
    "C"
    "CXX"
  PROJECT_BOARD_PORT_PATH "${board_root}/${board}/cmsis_driver_examples/usb_dev"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_macro(
  CC "-DUSB_STACK_FREERTOS"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "examples/cmsis_driver_examples/usb_dev"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "examples/cmsis_driver_examples/usb_dev/cmsis_usbd_validation.c"
    "examples/cmsis_driver_examples/usb_dev/cmsis_usbd_validation.h"
    "examples/cmsis_driver_examples/usb_dev/freertos_hooks.c"
)

# CMSIS-RTOS2 FreeRTOS wrapper
mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "examples/cmsis_driver_examples/usb_dev/cmsis_rtos2/cmsis_os2.c"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "examples/cmsis_driver_examples/usb_dev/cmsis_rtos2"
)

# CMSIS DV framework source
mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "examples/cmsis_driver_examples/usb_dev/cmsis_dv_source/cmsis_dv.c"
    "examples/cmsis_driver_examples/usb_dev/cmsis_dv_source/DV_Framework.c"
    "examples/cmsis_driver_examples/usb_dev/cmsis_dv_source/DV_Report.c"
    "examples/cmsis_driver_examples/usb_dev/cmsis_dv_source/DV_USBD.c"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "examples/cmsis_driver_examples/usb_dev/cmsis_dv_source/include"
)

include("${SdkRootDirPath}/${board_root}/${board}/cmsis_driver_examples/usb_dev/reconfig.cmake" OPTIONAL)

mcux_convert_binary(
  BINARY "${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin"
)
