#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(executorch_lib LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/eiq_examples/executorch_lib PROJECT_TYPE LIBRARY)

mcux_add_cmakelists(${SdkRootDirPath}/middleware/eiq/executorch/examples/nxp/runtime/imxrt700 OPTIONAL)

mcux_add_armgcc_configuration(
  CC "-std=c99"
  CX "-std=c++17"
)
mcux_add_mcux_configuration(
  CC "-std=gnu99"
  CX "-std=gnu++17"
)


mcux_add_armgcc_configuration(
  CC "-Wall -fno-common -ffunction-sections -fdata-sections -fno-builtin -mthumb -mapcs -DNDEBUG -O3 -DET_LOG_ENABLED=0"
  CX "-Wall -fno-common -ffunction-sections -fdata-sections -fno-builtin -mthumb -mapcs -fno-rtti -fno-exceptions -DNDEBUG -O3 -DET_LOG_ENABLED=0"
)


mcux_add_xtensa_configuration(
  CC "-Wall -fno-common -ffunction-sections -fdata-sections -fno-builtin -DNDEBUG  -O3 -mlongcalls -DET_LOG_ENABLED=0"
  CX "-Wall -fno-common -ffunction-sections -fdata-sections -fno-builtin -fno-rtti -fno-exceptions -DNDEBUG -O3 -mlongcalls -DET_LOG_ENABLED=0"
)


if(${CONFIG_MCUX_HW_CORE} STREQUAL "cm33" OR ${CONFIG_MCUX_HW_CORE} STREQUAL "cm33f")
  mcux_add_armgcc_configuration(
    CC "-mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16"
    CX "-mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16"
  )
endif()

include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/executorch_lib/reconfig.cmake OPTIONAL)
