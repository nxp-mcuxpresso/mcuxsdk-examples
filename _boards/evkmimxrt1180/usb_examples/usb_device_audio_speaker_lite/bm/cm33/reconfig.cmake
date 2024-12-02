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

mcux_add_armgcc_configuration(
  CC "-O2       -Wno-array-bounds"
)

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES "middleware/usb/example/boards/${board}/usb_device_audio_speaker_lite/bm/${core_id}"
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
  CC "-DUSING_SAI -DDATA_SECTION_IS_CACHEABLE=0"
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
    "middleware/usb/example/boards/${board}/usb_device_audio_speaker_lite/bm/${core_id}/app.h"
    "middleware/usb/example/boards/${board}/usb_device_audio_speaker_lite/bm/${core_id}/hardware_init.c"
    "middleware/usb/example/boards/${board}/usb_device_audio_speaker_lite/bm/${core_id}/pin_mux.c"
    "middleware/usb/example/boards/${board}/usb_device_audio_speaker_lite/bm/${core_id}/pin_mux.h"
    "middleware/usb/example/boards/${board}/usb_device_audio_speaker_lite/bm/${core_id}/usb_audio_config.h"
)

mcux_remove_armgcc_configuration(
  CC "-O0"
)
