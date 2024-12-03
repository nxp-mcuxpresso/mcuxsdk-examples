cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  dev_composite_cdc_audio_unified_freertos
  LANGUAGES
    "ASM"
    "C"
    "CXX"
  PROJECT_BOARD_PORT_PATH "examples/_boards/${board}/usb_examples/usb_device_composite_cdc_audio_unified/freertos"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_iar_configuration(
  LD "--semihosting"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/usb_device_composite_cdc_audio_unified/freertos"
)

mcux_add_macro(
  CC "-DHAL_AUDIO_ISR_PRIORITY=0 -DUSB_STACK_FREERTOS"
)

mcux_add_mdk_configuration(
  LD "--library_type=microlib"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/usb_device_composite_cdc_audio_unified/freertos/audio_data_dmic.c"
    "middleware/usb/example/usb_device_composite_cdc_audio_unified/freertos/audio_data_dmic.h"
    "middleware/usb/example/usb_device_composite_cdc_audio_unified/freertos/audio_unified.c"
    "middleware/usb/example/usb_device_composite_cdc_audio_unified/freertos/audio_unified.h"
    "middleware/usb/example/usb_device_composite_cdc_audio_unified/freertos/composite.c"
    "middleware/usb/example/usb_device_composite_cdc_audio_unified/freertos/composite.h"
    "middleware/usb/example/usb_device_composite_cdc_audio_unified/freertos/usb_device_descriptor.c"
    "middleware/usb/example/usb_device_composite_cdc_audio_unified/freertos/usb_device_descriptor.h"
    "middleware/usb/example/usb_device_composite_cdc_audio_unified/freertos/virtual_com.c"
    "middleware/usb/example/usb_device_composite_cdc_audio_unified/freertos/virtual_com.h"
)

include("${SdkRootDirPath}/examples/_boards/${board}/usb_examples/usb_device_composite_cdc_audio_unified/freertos/reconfig.cmake" OPTIONAL)

mcux_convert_binary(
  BINARY "${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin"
)
