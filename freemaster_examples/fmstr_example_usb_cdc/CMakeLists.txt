cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(fmstr_example_usb_cdc LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/freemaster_examples/fmstr_usb_cdc)

include(${SdkRootDirPath}/CMakeLists.txt)

# Include optional board specific configuration cmake file
include(${SdkRootDirPath}/examples/_boards/${board}/freemaster_examples/fmstr_usb_cdc/reconfig.cmake OPTIONAL)

# Load common source files
include(../common/CMakeLists.txt)
include(../common/example/common/CMakeLists.txt)

# Add USB stack macro
mcux_add_macro(
    CC "USB_STACK_BM=1"
)

# Add USB include paths
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
        middleware/freemaster/src/support/mcuxsdk_usb
)

# Add example USB source files
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        middleware/freemaster/src/support/mcuxsdk_usb/freemaster_usb.h
        middleware/freemaster/src/support/mcuxsdk_usb/freemaster_usb.c
        middleware/freemaster/src/support/mcuxsdk_usb/freemaster_usb_device_descriptor.h
        middleware/freemaster/src/support/mcuxsdk_usb/freemaster_usb_device_descriptor.c
        middleware/freemaster/src/support/mcuxsdk_usb/usb_device_descriptor.h
)