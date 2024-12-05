mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES  examples/_boards/${board}/eiq_examples/tflm_kws/npu
  )
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/_boards/${board}/eiq_examples/tflm_kws/npu/model_ds_cnn_ops_npu.cpp
  examples/_boards/${board}/eiq_examples/tflm_kws/npu/model_data.h
  )

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  TARGETS flash_debug flash_release
  LINKER devices/MCX/MCXN/MCXN947/iar/MCXN947_cm33_core0_flash.icf
  )
mcux_remove_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  TARGETS flash_debug flash_release
  LINKER devices/MCX/MCXN/MCXN947/arm/MCXN947_cm33_core0_flash.scf
  )
mcux_remove_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  TARGETS flash_debug flash_release
  LINKER devices/MCX/MCXN/MCXN947/gcc/MCXN947_cm33_core0_flash.ld
  )

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
  BASE_PATH ${SdkRootDirPath}
  TARGETS flash_debug flash_release
  LINKER examples/_boards/${board}/eiq_examples/tflm_kws/cm33_core0/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
  )
mcux_add_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  TARGETS flash_debug flash_release
  LINKER examples/_boards/${board}/eiq_examples/tflm_kws/cm33_core0/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
  )
mcux_add_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  TARGETS flash_debug flash_release
  LINKER examples/_boards/${board}/eiq_examples/tflm_kws/cm33_core0/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
  )


mcux_add_iar_configuration(
  CC "--use_cmsis_dsp\
  --diag_suppress=Pe167\
  --diag_suppress=Pe260,Pe1031"
  )
mcux_remove_iar_configuration(
  TARGETS flash_debug flash_release
  CC "-Oh -On"
  )
mcux_add_iar_configuration(
  TARGETS flash_debug flash_release
  CC "-Ohs"
  )
mcux_add_mdk_configuration(
  LD "--diag_suppress=6439,6776"
  )
mcux_add_mdk_configuration(
  TARGETS flash_debug flash_release
  CC "-O3"
  )
mcux_add_armgcc_configuration(
  TARGETS flash_debug flash_release
  CC "-O3"
  CX "-O3"
  )

mcux_add_macro(
  CC "-DSDK_DEBUGCONSOLE_UART\
  -DARM_MATH_CM33\
  -D__FPU_PRESENT=1"
  CX "-DSDK_DEBUGCONSOLE_UART\
  -DARM_MATH_CM33\
  -D__FPU_PRESENT=1"
  )

mcux_remove_iar_configuration(
  TARGETS flash_debug flash_release
  CC "-Om"
  )
mcux_remove_mdk_configuration(
  CC "-fshort-wchar"
  CX "-fshort-wchar"
  )
mcux_remove_mdk_configuration(
  TARGETS flash_debug flash_release
  CC "-Oz"
  )
mcux_remove_armgcc_configuration(
  TARGETS flash_debug flash_release
  CC "-Os"
  CX "-Os"
  )

mcux_add_iar_configuration(
  LD "--config_def=__heap_size__=0x8000\
  --config_def=__stack_size__=0x2000"
  )
mcux_add_mdk_configuration(
  LD "--predefine=\"-D__heap_size__=0x8000\"\
  --predefine=\"-D__stack_size__=0x2000\""
  )
mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x8000\
  -Xlinker --defsym=__stack_size__=0x2000"
  )
