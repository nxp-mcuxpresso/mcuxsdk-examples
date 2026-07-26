# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_macro(
  CC "-DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1 -DBOARD_USE_PCAL6524=1 -DSDK_I2C_BASED_COMPONENT_USED=1"
)

mcux_add_source(
  BASE_PATH ${SdkRootDirPath} SOURCES
  ${board_root}/${board}/lwip_examples/common/enet/hardware_init.c
  ${board_root}/${board}/lwip_examples/common/enet/app.h)

mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES
                 ${board_root}/${board}/lwip_examples/common/enet)

mcux_add_armgcc_configuration(CC "-fno-branch-count-reg")

if(CONFIG_TOOLCHAIN STREQUAL "iar")
  set_source_files_properties(${SdkRootDirPath}/examples/_boards/${board}/common/clock/clock_config.c PROPERTIES COMPILE_OPTIONS "-On")
endif()
