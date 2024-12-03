mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES  examples/_boards/${board}/eiq_examples/tflm_cifar10/pcq_npu
  )
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/_boards/${board}/eiq_examples/tflm_cifar10/pcq_npu/model_cifarnet_ops_npu.cpp
  examples/_boards/${board}/eiq_examples/tflm_cifar10/pcq_npu/model_data.h
  )

mcux_remove_iar_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.icf
  )
mcux_remove_iar_linker_script(
  TARGETS flash_debug flash_release
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
  )
mcux_remove_mdk_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.scf
  )
mcux_remove_mdk_linker_script(
  TARGETS flash_debug flash_release
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
  )
mcux_remove_armgcc_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
  )
mcux_remove_armgcc_linker_script(
  TARGETS flash_debug flash_release
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
  )

mcux_add_armgcc_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/mimxrt700evk/eiq_examples/tflm_cifar10/cm33_core0/gcc/MIMXRT798Sxxxx_cm33_core0_ram.ld
  )
mcux_add_armgcc_linker_script(
  TARGETS flash_debug flash_release
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/mimxrt700evk/eiq_examples/tflm_cifar10/cm33_core0/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld
  )
mcux_add_mdk_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/mimxrt700evk/eiq_examples/tflm_cifar10/cm33_core0/arm/MIMXRT798Sxxxx_cm33_core0_ram.scf
  )
mcux_add_mdk_linker_script(
  TARGETS flash_debug flash_release
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/mimxrt700evk/eiq_examples/tflm_cifar10/cm33_core0/arm/MIMXRT798Sxxxx_cm33_core0_flash.scf
  )
mcux_add_iar_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/mimxrt700evk/eiq_examples/tflm_cifar10/cm33_core0/iar/MIMXRT798Sxxxx_cm33_core0_ram.icf
  )
mcux_add_iar_linker_script(
  TARGETS flash_debug flash_release
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/mimxrt700evk/eiq_examples/tflm_cifar10/cm33_core0/iar/MIMXRT798Sxxxx_cm33_core0_flash.icf
  )

mcux_add_iar_configuration(
  CC "--diag_suppress=Pe167\
  --diag_suppress=Pe260,Pe1031"
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
  -DTENSORARENA_NONCACHE=1\
  -D__FPU_PRESENT=1"
  CX "-DSDK_DEBUGCONSOLE_UART\
  -DARM_MATH_CM33\
  -DTENSORARENA_NONCACHE=1\
  -D__FPU_PRESENT=1"
  )

mcux_remove_iar_configuration(
  TARGETS release
  CC "-Om -Oh"
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
  LD "--config_def=__heap_size__=0x8000\
  --config_def=__stack_size__=0x2000"
  )
mcux_add_mdk_configuration(
  LD "--predefine=-D__heap_size__=0x8000\
  --predefine=-D__stack_size__=0x2000"
  )
mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x8000\
  -Xlinker --defsym=__stack_size__=0x2000"
  )
