mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x400"
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
  INCLUDES "${board_root}/${board}/usb_examples/usb_device_composite_cdc_msc_disk/bm"
)

mcux_remove_macro(
  CC "DATA_SECTION_IS_CACHEABLE=1"
)
 
mcux_add_macro(
  CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 -DUSB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
  TARGETS
    "hyperram_debug"
    "hyperram_release"
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

mcux_add_mdk_configuration(
  LD "--legacyalign       --diag_suppress=3912"
  TARGETS
    "flexspi_nor_debug"
    "flexspi_nor_release"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "${board_root}/${board}/sdmmc_config.c"
    "${board_root}/${board}/sdmmc_config.h"
    "${board_root}/${board}/usb_examples/usb_device_composite_cdc_msc_disk/bm/hardware_init.c"
    "${board_root}/${board}/usb_examples/usb_device_composite_cdc_msc_disk/bm/usb_device_msc_config.h"
    "middleware/usb/utility/usb_disk_adapter.h"
    "middleware/usb/utility/usb_sdcard_adapter.c"
)
