mcux_remove_iar_configuration(
  CC "-On"
  TARGETS
    "flexspi_nor_debug"
    "sdram_debug"
)

mcux_add_iar_configuration(
  CC "-Om"
  TARGETS
    "flexspi_nor_debug"
    "sdram_debug"
)

mcux_remove_mdk_configuration(
  CC "-O0"
  TARGETS
    "flexspi_nor_debug"
    "sdram_debug"
)

mcux_add_mdk_configuration(
  CC "-O1"
  TARGETS
    "flexspi_nor_debug"
    "sdram_debug"
)

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

mcux_add_armgcc_configuration(
  CC "-O1"
  TARGETS "sdram_debug"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES
    "middleware/usb/example/boards/${board}/usb_device_cdc_vnic/freertos"
    "middleware/usb/example/usb_device_cdc_vnic/freertos/enet_adapter/kinetis"
)

mcux_remove_macro(
  CC "DATA_SECTION_IS_CACHEABLE=1"
)
 
mcux_add_macro(
  CC "-DUSB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE=1 -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1"
  TARGETS
    "flexspi_nor_sdram_debug"
    "flexspi_nor_sdram_release"
    "sdram_debug"
    "sdram_release"
  TOOLCHAINS
    "armgcc"
    "iar"
    "mdk"
)

mcux_add_macro(
  CC "-DDEBUG_CONSOLE_ASSERT_DISABLE=1 -DNDEBUG"
  TARGETS "sdram_debug"
  TOOLCHAINS "armgcc"
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
  CC "-DDEBUG_CONSOLE_ASSERT_DISABLE=1"
  TARGETS "sdram_debug"
  TOOLCHAINS "iar"
)

mcux_add_macro(
  CC "-DFSL_FEATURE_PHYKSZ8081_USE_RMII50M_MODE"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/boards/${board}/usb_device_cdc_vnic/freertos/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_cdc_vnic/freertos/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_cdc_vnic/freertos/pin_mux.h"
    "middleware/usb/example/usb_device_cdc_vnic/freertos/enet_adapter/kinetis/virtual_nic_enet_adapter.c"
    "middleware/usb/example/usb_device_cdc_vnic/freertos/enet_adapter/kinetis/virtual_nic_enet_adapter.h"
    "middleware/usb/example/usb_device_cdc_vnic/freertos/enet_adapter/kinetis/virtual_nic_enetif.c"
    "middleware/usb/example/usb_device_cdc_vnic/freertos/enet_adapter/kinetis/virtual_nic_enetif.h"
)

mcux_remove_armgcc_configuration(
  CC "-O0"
  TARGETS "sdram_debug"
)
