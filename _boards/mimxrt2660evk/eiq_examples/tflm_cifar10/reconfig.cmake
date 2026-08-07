# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES  ${board_root}/${board}/eiq_examples/tflm_cifar10/npu
)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES ${board_root}/${board}/eiq_examples/tflm_cifar10/npu/model_cifarnet_ops_npu.cpp
  ${board_root}/${board}/eiq_examples/tflm_cifar10/npu/model_data.h
)

mcux_remove_armgcc_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_ram.ld
)
mcux_add_armgcc_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/mimxrt2660evk/eiq_examples/tflm_cifar10/gcc/MIMXRT2663xxxxx_ram.ld
)

mcux_remove_armgcc_linker_script(
  TARGETS psram_txt_debug psram_txt_release
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_psram_txt.ld
)
mcux_add_armgcc_linker_script(
  TARGETS psram_txt_debug psram_txt_release
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/mimxrt2660evk/eiq_examples/tflm_cifar10/gcc/MIMXRT2663xxxxx_psram_txt.ld
)

mcux_remove_iar_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_ram.icf
)
mcux_add_iar_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/mimxrt2660evk/eiq_examples/tflm_cifar10/iar/MIMXRT2663xxxxx_ram.icf
)

mcux_remove_iar_linker_script(
  TARGETS psram_txt_debug psram_txt_release
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_psram_txt.icf
)
mcux_add_iar_linker_script(
  TARGETS psram_txt_debug psram_txt_release
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/mimxrt2660evk/eiq_examples/tflm_cifar10/iar/MIMXRT2663xxxxx_psram_txt.icf
)

mcux_remove_iar_linker_script(
  TARGETS xspi_nor_psram_debug xspi_nor_psram_release
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_xspi_nor_psram.icf
)
mcux_add_iar_linker_script(
  TARGETS xspi_nor_psram_debug xspi_nor_psram_release
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/mimxrt2660evk/eiq_examples/tflm_cifar10/iar/MIMXRT2663xxxxx_xspi_nor_psram.icf
)

mcux_remove_armgcc_linker_script(
  TARGETS xspi_nor_psram_debug xspi_nor_psram_release
  BASE_PATH ${SdkRootDirPath}
  LINKER devices/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_xspi_nor_psram.ld
)
mcux_add_armgcc_linker_script(
  TARGETS xspi_nor_psram_debug xspi_nor_psram_release
  BASE_PATH ${SdkRootDirPath}
  LINKER examples/_boards/mimxrt2660evk/eiq_examples/tflm_cifar10/gcc/MIMXRT2663xxxxx_xspi_nor_psram.ld
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

mcux_add_macro(
  CC "-DSDK_DEBUGCONSOLE_UART\
  -DARM_MATH_CM85\
  -DTENSORARENA_DATA=1\
  -D__FPU_PRESENT=1"
  CX "-DSDK_DEBUGCONSOLE_UART\
  -DARM_MATH_CM85\
  -DTENSORARENA_DATA=1\
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
  TARGETS release psram_release psram_txt_release
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
  CC "-Wno-stringop-overflow"
  LD "-Xlinker --defsym=__heap_size__=0x2000\
  -Xlinker --defsym=__stack_size__=0x2000"
)

mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES examples/eiq_examples/common/risc-v/riscv_elf.S
  TOOLCHAINS mdk armgcc mcux
)

mcux_add_iar_configuration(
  LD "--image_input=${SdkRootDirPath}/middleware/eiq/neutron/rt2660/NeutronFirmware.elf,_binary_NeutronFirmware_elf_start,.elfdata,16\
      --keep=_binary_NeutronFirmware_elf_start"
)
