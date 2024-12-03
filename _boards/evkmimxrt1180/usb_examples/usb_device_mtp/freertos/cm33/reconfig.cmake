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
  INCLUDES "middleware/usb/example/boards/${board}/usb_device_mtp/freertos/${core_id}"
)

mcux_remove_macro(
  CC "DATA_SECTION_IS_CACHEABLE=1"
)
 
mcux_add_macro(
  CC "-DUSB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
  TARGETS
    "hyperram_debug"
    "hyperram_release"
)

mcux_add_macro(
  CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 -DDATA_SECTION_IS_CACHEABLE=0"
)

mcux_remove_macro(
  CC "FSL_SDK_ENABLE_DRIVER_CACHE_CONTROL"
  TOOLCHAINS "iar"
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
    "examples/_boards/${board}/sdmmc_config.c"
    "examples/_boards/${board}/sdmmc_config.h"
    "middleware/usb/example/boards/${board}/usb_device_mtp/freertos/${core_id}/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_mtp/freertos/${core_id}/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_mtp/freertos/${core_id}/pin_mux.h"
    "middleware/usb/example/boards/${board}/usb_device_mtp/freertos/${core_id}/usb_device_mtp_config.h"
)
