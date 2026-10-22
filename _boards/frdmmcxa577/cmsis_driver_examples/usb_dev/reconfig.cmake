mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x1000"
)

mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__stack_size__=0x2000"
)

mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x1000"
)

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x2000"
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__heap_size__=0x1000\""
)

mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x2000\""
)

mcux_add_macro(
  CC "SDK_DEBUGCONSOLE=0"
)

set(_board_cmsis_val_dir "${board_root}/${board}/cmsis_driver_examples/usb_dev")

mcux_add_include(
  BASE_PATH "${SdkRootDirPath}"
  INCLUDES
    "${_board_cmsis_val_dir}"
    "arch/arm/CMSIS/RTOS2/Include"
    "arch/arm/CMSIS/Driver/Include"
    "middleware/usb/include"
)

mcux_add_armgcc_configuration(
  CC "-include ${SdkRootDirPath}/${_board_cmsis_val_dir}/RTE_Components.h"
)

mcux_add_iar_configuration(
  CC "--preinclude ${SdkRootDirPath}/${_board_cmsis_val_dir}/RTE_Components.h"
)

mcux_add_iar_configuration(
  CC "--diag_suppress=Pe186"
)

mcux_add_mdk_configuration(
  CC "--preinclude ${SdkRootDirPath}/${_board_cmsis_val_dir}/RTE_Components.h"
)

mcux_add_source(
  BASE_PATH "${SdkRootDirPath}"
  SOURCES
    "${_board_cmsis_val_dir}/hardware_init.c"
)
