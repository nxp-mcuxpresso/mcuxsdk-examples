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

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES
    "middleware/usb/example/boards/${board}/usb_device_cdc_vnic_lite/bm"
    "middleware/usb/example/usb_device_cdc_vnic_lite/bm/enet_adapter/lpc"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/boards/${board}/usb_device_cdc_vnic_lite/bm/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_cdc_vnic_lite/bm/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_cdc_vnic_lite/bm/pin_mux.h"
    "middleware/usb/example/usb_device_cdc_vnic_lite/bm/enet_adapter/lpc/virtual_nic_enet_adapter.c"
    "middleware/usb/example/usb_device_cdc_vnic_lite/bm/enet_adapter/lpc/virtual_nic_enet_adapter.h"
    "middleware/usb/example/usb_device_cdc_vnic_lite/bm/enet_adapter/lpc/virtual_nic_enetif.c"
    "middleware/usb/example/usb_device_cdc_vnic_lite/bm/enet_adapter/lpc/virtual_nic_enetif.h"
)
