mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES  examples/_boards/${board}/eiq_examples/tflm_cifar10/pcq_npu
  )
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/_boards/${board}/eiq_examples/tflm_cifar10/pcq_npu/model_cifarnet_ops_npu.cpp
  examples/_boards/${board}/eiq_examples/tflm_cifar10/pcq_npu/model_data.h
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
  LINKER examples/_boards/${board}/eiq_examples/tflm_label_image/cm33_core0/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
  )
mcux_add_mdk_linker_script(
  BASE_PATH ${SdkRootDirPath}
  TARGETS flash_debug flash_release
  LINKER examples/_boards/${board}/eiq_examples/tflm_label_image/cm33_core0/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
  )
mcux_add_armgcc_linker_script(
  BASE_PATH ${SdkRootDirPath}
  TARGETS flash_debug flash_release
  LINKER examples/_boards/${board}/eiq_examples/tflm_label_image/cm33_core0/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
  )

mcux_add_iar_configuration(
  CC "--diag_suppress Pe260,Pe1031"
  )
mcux_remove_iar_configuration(
  TARGETS release
  CC "-Oh"
  )
mcux_add_iar_configuration(
  TARGETS release
  CC "-Ohs"
  )
mcux_add_mdk_configuration(
  LD "--diag_suppress=6439,6776"
  )
mcux_add_mdk_configuration(
  TARGETS release
  CC "-O3"
  )
mcux_add_armgcc_configuration(
  TARGETS release
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
  TARGETS release
  CC "-Om"
  )
mcux_remove_mdk_configuration(
  CC "-fshort-wchar"
  CX "-fshort-wchar"
  )
mcux_remove_mdk_configuration(
  TARGETS release
  CC "-Oz"
  )
mcux_remove_armgcc_configuration(
  TARGETS release
  CC "-Os"
  CX "-Os"
  )

mcux_add_iar_configuration(
  LD "--config_def=__stack_size__=0x2000"
  )
mcux_add_mdk_configuration(
  LD "--predefine=\"-D__stack_size__=0x2000\""
  )
mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__stack_size__=0x2000"
  )
