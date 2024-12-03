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

mcux_add_armgcc_linker_script(
  BASE_PATH "${SdkRootDirPath}"
  LINKER "middleware/usb/example/boards/${board}/usb_device_dfu/bm/linker/gcc/MIMXRT1024xxxxx_sdram.ld"
  TARGETS
    "sdram_debug"
    "sdram_release"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/boards/${board}/usb_device_dfu/bm"
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

mcux_remove_macro(
  CC "DATA_SECTION_IS_CACHEABLE=1"
)
 
mcux_add_macro(
  CC "-DUSB_DEVICE_CONFIG_BUFFER_PROPERTY_CACHEABLE=1"
  TARGETS
    "sdram_debug"
    "sdram_release"
  TOOLCHAINS
    "armgcc"
    "iar"
    "mdk"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/boards/${board}/usb_device_dfu/bm/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_dfu/bm/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_dfu/bm/pin_mux.h"
    "middleware/usb/example/boards/${board}/usb_device_dfu/bm/usb_device_dfu_config.h"
    "middleware/usb/utility/usb_dfu_ram.c"
    "middleware/usb/utility/usb_flash.h"
)

mcux_remove_armgcc_linker_script(
  BASE_PATH "${SdkRootDirPath}"
  LINKER "devices/RT/RT1020/MIMXRT1024/gcc/MIMXRT1024xxxxx_sdram.ld"
  TARGETS
    "sdram_debug"
    "sdram_release"
)
