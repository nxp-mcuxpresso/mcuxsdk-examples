# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x8000"
)

mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__stack_size__=0x1000"
)

mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x8000"
)

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x1000"
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__heap_size__=0x8000\""
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x1000\""
)

mcux_add_macro(
  CC "-DSDK_DEBUGCONSOLE=1"
)

mcux_add_macro(
  CC "-DUSB_HOST_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
  TARGETS
    "psram_debug"
    "psram_release"
    "psram_txt_debug"
    "psram_txt_release"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "${board_root}/${board}/lwip_examples/lwip_dhcp_usb/freertos"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "${board_root}/${board}/lwip_examples/lwip_dhcp_usb/freertos/app.h"
    "${board_root}/${board}/lwip_examples/lwip_dhcp_usb/freertos/hardware_init.c"
)

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/_boards/${board}/lwip_examples/lwip_dhcp_usb/freertos/freertos_config/FreeRTOSConfig.h
)

# BEFORE: the app dir (which carries its own FreeRTOSConfig.h) is already on the
# include path; the kernel must resolve to this board's copy.
target_include_directories(${MCUX_SDK_PROJECT_NAME} BEFORE PRIVATE
  ${SdkRootDirPath}/examples/_boards/${board}/lwip_examples/lwip_dhcp_usb/freertos/freertos_config)
