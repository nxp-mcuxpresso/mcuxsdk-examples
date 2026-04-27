mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__stack_size__=0x1000"
)

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x1000"
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x1000\""
)

mcux_add_macro(
  CC "-DUSB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE=1 \
      -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES
    "${board_root}/${board}/usb_examples/usb_device_composite_cdc_msc_disk/bm"
    "${board_root}/${board}/usb_examples/usb_device_composite_cdc_msc_disk/bm"
    "${board_root}/${board}/usb_examples/usb_device_composite_cdc_msc_disk/bm/${core_id}"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "${board_root}/${board}/sdmmc_config.c"
    "${board_root}/${board}/sdmmc_config.h"
    "${board_root}/${board}/usb_examples/usb_device_composite_cdc_msc_disk/bm/${core_id}/hardware_init.c"
    "${board_root}/${board}/usb_examples/usb_device_composite_cdc_msc_disk/bm/${core_id}/usb_device_msc_config.h"
    "middleware/usb/utility/usb_disk_adapter.h"
    "middleware/usb/utility/usb_sdcard_adapter.c"
)
