mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x1000"
)

mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__stack_size__=0x1000"
)

mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x1000"
)

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x1000"
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__heap_size__=0x1000\""
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x1000\""
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES
    "middleware/usb/example/boards/${board}/usb_device_cdc_vnic/bm/${core_id}"
    "middleware/usb/example/usb_device_cdc_vnic/bm/enet_adapter/kinetis"
)

mcux_remove_macro(
  CC "DATA_SECTION_IS_CACHEABLE=1"
)
 
mcux_add_macro(
  CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 -DUSB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
  TARGETS
    "sdram_debug"
    "sdram_release"
  TOOLCHAINS
    "armgcc"
    "iar"
    "mdk"
)

mcux_add_macro(
  CC "-DDATA_SECTION_IS_CACHEABLE=0"
  TARGETS
    "debug"
    "flexspi_nor_debug"
    "flexspi_nor_release"
    "release"
  TOOLCHAINS
    "armgcc"
    "iar"
    "mdk"
)

mcux_add_macro(
  CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE"
)

mcux_add_mdk_configuration(
  LD "--legacyalign       --diag_suppress=3912"
  TARGETS
    "flexspi_nor_debug"
    "flexspi_nor_release"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/boards/${board}/usb_device_cdc_vnic/bm/${core_id}/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_cdc_vnic/bm/${core_id}/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_cdc_vnic/bm/${core_id}/pin_mux.h"
    "middleware/usb/example/usb_device_cdc_vnic/bm/enet_adapter/kinetis/virtual_nic_enet_adapter.c"
    "middleware/usb/example/usb_device_cdc_vnic/bm/enet_adapter/kinetis/virtual_nic_enet_adapter.h"
    "middleware/usb/example/usb_device_cdc_vnic/bm/enet_adapter/kinetis/virtual_nic_enetif.c"
    "middleware/usb/example/usb_device_cdc_vnic/bm/enet_adapter/kinetis/virtual_nic_enetif.h"
)
