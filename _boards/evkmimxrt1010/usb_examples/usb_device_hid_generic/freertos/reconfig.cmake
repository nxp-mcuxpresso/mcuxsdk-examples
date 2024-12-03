mcux_remove_iar_configuration(
  CC "-On"
  TARGETS "flexspi_nor_debug"
)

mcux_add_iar_configuration(
  CC "-Om"
  TARGETS "flexspi_nor_debug"
)

mcux_remove_mdk_configuration(
  CC "-O0"
  TARGETS "flexspi_nor_debug"
)

mcux_add_mdk_configuration(
  CC "-O1"
  TARGETS "flexspi_nor_debug"
)

mcux_remove_iar_linker_script(
  BASE_PATH "${SdkRootDirPath}"
  LINKER "devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor.icf"
  TARGETS
    "flexspi_nor_debug"
    "flexspi_nor_release"
)

mcux_remove_armgcc_linker_script(
  BASE_PATH "${SdkRootDirPath}"
  LINKER "devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor.ld"
  TARGETS
    "flexspi_nor_debug"
    "flexspi_nor_release"
)

mcux_remove_mdk_linker_script(
  BASE_PATH "${SdkRootDirPath}"
  LINKER "devices/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor.scf"
  TARGETS
    "flexspi_nor_debug"
    "flexspi_nor_release"
)

mcux_add_iar_linker_script(
  BASE_PATH "${SdkRootDirPath}"
  LINKER "devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor_ocram.icf"
  TARGETS
    "flexspi_nor_debug"
    "flexspi_nor_release"
)

mcux_add_mdk_linker_script(
  BASE_PATH "${SdkRootDirPath}"
  LINKER "devices/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor_ocram.scf"
  TARGETS
    "flexspi_nor_debug"
    "flexspi_nor_release"
)

mcux_add_armgcc_linker_script(
  BASE_PATH "${SdkRootDirPath}"
  LINKER "devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flexspi_nor_ocram.ld"
  TARGETS
    "flexspi_nor_debug"
    "flexspi_nor_release"
)

mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x400"
)

mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__stack_size__=0x2000"
)

mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x400"
)

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x2000"
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__heap_size__=0x400\""
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x2000\""
)

mcux_add_armgcc_configuration(
  CC "-fno-toplevel-reorder"
  TARGETS "flexspi_nor_release"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/boards/${board}/usb_device_hid_generic/freertos"
)

mcux_add_macro(
  CC "-DDATA_SECTION_IS_CACHEABLE=0"
  TARGETS
    "debug"
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
    "flexspi_nor_debug"
    "flexspi_nor_release"
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
    "middleware/usb/example/boards/${board}/usb_device_hid_generic/freertos/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_hid_generic/freertos/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_hid_generic/freertos/pin_mux.h"
)
