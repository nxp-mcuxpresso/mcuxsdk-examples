mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x100"
)

mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__stack_size__=0x2000"
)

mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x100"
)

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x2000"
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__heap_size__=0x100\""
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x2000\""
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES
    "${board_root}/${board}/usb_examples/usb_device_cdc_vnic/freertos"
    "examples/usb_examples/usb_device_cdc_vnic/freertos/enet_adapter/lpc"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "${board_root}/${board}/usb_examples/usb_device_cdc_vnic/freertos/hardware_init.c"
    "${board_root}/${board}/usb_examples/usb_device_cdc_vnic/freertos/pin_mux.c"
    "${board_root}/${board}/usb_examples/usb_device_cdc_vnic/freertos/pin_mux.h"
    "examples/usb_examples/usb_device_cdc_vnic/freertos/enet_adapter/lpc/virtual_nic_enet_adapter.c"
    "examples/usb_examples/usb_device_cdc_vnic/freertos/enet_adapter/lpc/virtual_nic_enet_adapter.h"
    "examples/usb_examples/usb_device_cdc_vnic/freertos/enet_adapter/lpc/virtual_nic_enetif.c"
    "examples/usb_examples/usb_device_cdc_vnic/freertos/enet_adapter/lpc/virtual_nic_enetif.h"
)
