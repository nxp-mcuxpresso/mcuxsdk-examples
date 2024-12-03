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
  INCLUDES "middleware/usb/example/boards/${board}/usb_suspend_resume_device_hid_mouse/bm"
)

mcux_add_macro(
  CC "-DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1"
  TARGETS
    "flexspi_nor_debug"
    "flexspi_nor_release"
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

mcux_add_macro(
  AS "-D__STARTUP_INITIALIZE_RAMFUNCTION"
  TARGETS
    "flexspi_nor_debug"
    "flexspi_nor_release"
  TOOLCHAINS "armgcc"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/boards/${board}/usb_suspend_resume_device_hid_mouse/bm/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_suspend_resume_device_hid_mouse/bm/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_suspend_resume_device_hid_mouse/bm/pin_mux.h"
)
