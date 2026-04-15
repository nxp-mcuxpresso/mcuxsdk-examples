cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(tflm_lib LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH examples/_boards/${board}/eiq_examples/tflm_lib PROJECT_TYPE LIBRARY)

mcux_add_cmakelists(${SdkRootDirPath}/middleware/eiq/tensorflow-lite OPTIONAL)

mcux_add_iar_configuration(
  CC "--endian=little -e -Ohs -DNDEBUG  --diag_suppress=Pe167 --diag_suppress=Pe260,Pe1031,Pe144"
  CX "--endian=little -e  -DNDEBUG --diag_suppress=Pe167 --diag_suppress=Pe260,Pe1031,Pe144"
)
mcux_add_mdk_configuration(
	CC "-Oz -ffunction-sections -fno-common -fdata-sections -fshort-enums -mthumb -fno-rtti -DNDEBUG -Wno-unused-variable -Wno-nan-infinity-disabled"
	CX "-Oz -ffunction-sections -fno-common -fdata-sections -fshort-enums -mthumb -fno-rtti -DNDEBUG -Wno-unused-variable -Wno-nan-infinity-disabled"
)
mcux_add_armgcc_configuration(
  CC "-Wall -fno-common -ffunction-sections -fdata-sections -mthumb -mapcs -DNDEBUG  -O2 -Wno-unused-variable"
  CX "-Wall -fno-common -ffunction-sections -fdata-sections -mthumb -mapcs -fno-rtti -fno-exceptions -DNDEBUG -O2 -Wno-unused-variable"
)

if(${CONFIG_MCUX_HW_CORE} STREQUAL "cm85" OR ${CONFIG_MCUX_HW_CORE} STREQUAL "cm85f")
  mcux_add_iar_configuration(
    CC "--cpu=Cortex-M85 --fpu=VFPv5_d16 -DARM_MATH_MVEI -DARM_MATH_AUTOVECTORIZE"
    CX "--cpu=Cortex-M85 --fpu=VFPv5_d16 -DARM_MATH_MVEI -DARM_MATH_AUTOVECTORIZE"
  )
  mcux_add_mdk_configuration(
    CC "-mcpu=cortex-m85 -mfpu=fpv5-sp-d16"
    CX "-mcpu=cortex-m85 -mfpu=fpv5-sp-d16"
  )
  mcux_add_armgcc_configuration(
    CC "-mcpu=cortex-m85 -mfloat-abi=hard -mfpu=fpv5-d16"
    CX "-mcpu=cortex-m85 -mfloat-abi=hard -mfpu=fpv5-d16"
  )
endif()
if(${CONFIG_MCUX_HW_CORE} STREQUAL "cm33" OR ${CONFIG_MCUX_HW_CORE} STREQUAL "cm33f")
  mcux_add_iar_configuration(
    CC "--cpu=cortex-m33.no_se --fpu=VFPv5_sp"
    CX "--cpu=cortex-m33.no_se --fpu=VFPv5_sp" 
  )
  mcux_add_mdk_configuration(
    CC "-mcpu=cortex-m33 -mfpu=fpv5-sp-d16"
    CX "-mcpu=cortex-m33 -mfpu=fpv5-sp-d16"
  )
  mcux_add_armgcc_configuration(
    CC "-mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16"
    CX "-mcpu=cortex-m33 -mfloat-abi=hard -mfpu=fpv5-sp-d16"
  )
endif()
if(${CONFIG_MCUX_HW_CORE} STREQUAL "cm7" OR ${CONFIG_MCUX_HW_CORE} STREQUAL "cm7f")
  mcux_add_iar_configuration(
    CC "--cpu=cortex-m7 --fpu=VFPv5_d16"
    CX "--cpu=cortex-m7 --fpu=VFPv5_d16" 
  )
  mcux_add_mdk_configuration(
    CC "-mcpu=cortex-m7 -mfpu=fpv5-d16"
    CX "-mcpu=cortex-m7 -mfpu=fpv5-d16"
  )
  mcux_add_armgcc_configuration(
    CC "-mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16"
    CX "-mcpu=cortex-m7 -mfloat-abi=hard -mfpu=fpv5-d16"
  )
endif()
if(${CONFIG_MCUX_HW_CORE} STREQUAL "cm4" OR ${CONFIG_MCUX_HW_CORE} STREQUAL "cm4f")
  mcux_add_iar_configuration(
    CC "--cpu=cortex-m4 --fpu=VFPv4_sp"
    CX "--cpu=cortex-m4 --fpu=VFPv4_sp" 
  )
  mcux_add_mdk_configuration(
    CC "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16"
    CX "-mcpu=cortex-m4 -mfpu=fpv4-sp-d16"
  )
  mcux_add_armgcc_configuration(
    CC "-mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16"
    CX "-mcpu=cortex-m4 -mfloat-abi=hard -mfpu=fpv4-sp-d16"
  )
endif()

include(${SdkRootDirPath}/${board_root}/${board}/eiq_examples/tflm_lib/reconfig.cmake OPTIONAL)
