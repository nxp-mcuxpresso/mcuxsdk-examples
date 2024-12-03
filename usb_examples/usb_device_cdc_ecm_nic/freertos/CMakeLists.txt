cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  dev_cdc_ecm_nic_freertos
  LANGUAGES
    "ASM"
    "C"
    "CXX"
  PROJECT_BOARD_PORT_PATH "examples/_boards/${board}/usb_examples/usb_device_cdc_ecm_nic/freertos"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/usb_device_cdc_ecm_nic/freertos"
)

mcux_add_macro(
  CC "-DUSB_STACK_FREERTOS"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/usb_device_cdc_ecm_nic/freertos/usb_device_descriptor.c"
    "middleware/usb/example/usb_device_cdc_ecm_nic/freertos/usb_device_descriptor.h"
    "middleware/usb/example/usb_device_cdc_ecm_nic/freertos/app.c"
    "middleware/usb/example/usb_device_cdc_ecm_nic/freertos/app.h"
)

include("${SdkRootDirPath}/examples/_boards/${board}/usb_examples/usb_device_cdc_ecm_nic/freertos/reconfig.cmake" OPTIONAL)

mcux_convert_binary(
  BINARY "${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin"
)
