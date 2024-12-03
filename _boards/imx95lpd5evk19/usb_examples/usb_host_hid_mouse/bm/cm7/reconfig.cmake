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

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/boards/${board}/usb_host_hid_mouse/bm/${core_id}"
)

mcux_add_macro(
  CC "-DDATA_SECTION_IS_CACHEABLE=0\
      -DSDK_I2C_BASED_COMPONENT_USED=1\
      -DBOARD_USE_PCAL6524=1"
  TARGETS
    "debug"
    "release"
  TOOLCHAINS
    "armgcc"
    "iar"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "middleware/usb/example/boards/${board}/usb_host_hid_mouse/bm/${core_id}/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_host_hid_mouse/bm/${core_id}/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_host_hid_mouse/bm/${core_id}/pin_mux.h"
)
