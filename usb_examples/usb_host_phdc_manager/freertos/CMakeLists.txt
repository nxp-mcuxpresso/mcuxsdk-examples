cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  host_phdc_manager_freertos
  LANGUAGES
    "ASM"
    "C"
    "CXX"
  PROJECT_BOARD_PORT_PATH "${board_root}/${board}/usb_examples/usb_host_phdc_manager/freertos"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_iar_configuration(
  LD "--semihosting"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "examples/usb_examples/usb_host_phdc_manager/freertos"
)

mcux_add_macro(
  CC "-DPRINTF_ADVANCED_ENABLE=1 -DPRINTF_FLOAT_ENABLE=1 -DUSB_STACK_FREERTOS"
)

mcux_add_mdk_configuration(
  LD "--library_type=microlib"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "examples/usb_examples/usb_host_phdc_manager/freertos/app.c"
    "examples/usb_examples/usb_host_phdc_manager/freertos/app.h"
    "examples/usb_examples/usb_host_phdc_manager/freertos/host_phdc_manager.c"
    "examples/usb_examples/usb_host_phdc_manager/freertos/host_phdc_manager.h"
    "examples/usb_examples/usb_host_phdc_manager/freertos/ieee11073_nomenclature.c"
    "examples/usb_examples/usb_host_phdc_manager/freertos/ieee11073_nomenclature.h"
    "examples/usb_examples/usb_host_phdc_manager/freertos/ieee11073_types.h"
)

include("${SdkRootDirPath}/${board_root}/${board}/usb_examples/usb_host_phdc_manager/freertos/reconfig.cmake" OPTIONAL)

mcux_convert_binary(
  BINARY "${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin"
)
