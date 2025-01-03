cmake_minimum_required(VERSION 3.22.0)

include("${SdkRootDirPath}/cmake/extension/mcux.cmake")

project(
  host_audio_speaker_bm
  LANGUAGES
    "ASM"
    "C"
    "CXX"
  PROJECT_BOARD_PORT_PATH "${board_root}/${board}/usb_examples/usb_host_audio_speaker/bm"
)

include("${SdkRootDirPath}/CMakeLists.txt")

mcux_add_iar_configuration(
  LD "--semihosting"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "examples/usb_examples/usb_host_audio_speaker/bm"
)

mcux_add_macro(
  CC "-DUSB_STACK_BM"
)

mcux_add_mdk_configuration(
  LD "--library_type=microlib"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "examples/usb_examples/usb_host_audio_speaker/bm/app.c"
    "examples/usb_examples/usb_host_audio_speaker/bm/app.h"
    "examples/usb_examples/usb_host_audio_speaker/bm/audio8k16S.c"
    "examples/usb_examples/usb_host_audio_speaker/bm/audio_speaker.c"
    "examples/usb_examples/usb_host_audio_speaker/bm/audio_speaker.h"
    "examples/usb_examples/usb_host_audio_speaker/bm/host_keypad.c"
    "examples/usb_examples/usb_host_audio_speaker/bm/host_keypad.h"
)

include("${SdkRootDirPath}/${board_root}/${board}/usb_examples/usb_host_audio_speaker/bm/reconfig.cmake" OPTIONAL)

mcux_convert_binary(
  BINARY "${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin"
)
