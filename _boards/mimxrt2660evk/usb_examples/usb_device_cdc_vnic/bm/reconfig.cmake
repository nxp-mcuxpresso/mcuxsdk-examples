# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x2000"
)

mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__stack_size__=0x2000"
)

mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x2000"
)

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x2000"
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__heap_size__=0x2000\""
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x2000\""
)

mcux_add_macro(
  CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
      -DBOARD_USE_PCAL6524=1\
      -DSDK_I2C_BASED_COMPONENT_USED=1\
      -DEXAMPLE_PHY_INTERFACE_RGMII"
)

mcux_add_macro(
  CC "-DUSB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
  TARGETS
    "psram_debug"
    "psram_release"
    "psram_txt_debug"
    "psram_txt_release"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES
    "${board_root}/${board}/usb_examples/usb_device_cdc_vnic/bm"
    "examples/usb_examples/usb_device_cdc_vnic/bm/enet_adapter/kinetis"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "${board_root}/${board}/usb_examples/usb_device_cdc_vnic/bm/hardware_init.c"
    "examples/usb_examples/usb_device_cdc_vnic/bm/enet_adapter/kinetis/virtual_nic_enet_adapter.c"
    "examples/usb_examples/usb_device_cdc_vnic/bm/enet_adapter/kinetis/virtual_nic_enet_adapter.h"
    "examples/usb_examples/usb_device_cdc_vnic/bm/enet_adapter/kinetis/virtual_nic_enetif.c"
    "examples/usb_examples/usb_device_cdc_vnic/bm/enet_adapter/kinetis/virtual_nic_enetif.h"
)
