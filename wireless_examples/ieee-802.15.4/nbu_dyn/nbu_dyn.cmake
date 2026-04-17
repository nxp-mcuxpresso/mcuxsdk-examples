# Copyright 2025-2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/ieee-802.15.4/examples/nbu_ble_15_4_dyn
    SOURCES
        nbu_15_4_dyn.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/ieee-802.15.4/examples/nbu_ble_15_4_dyn
    PREINCLUDE TRUE
    SOURCES
        app_preinclude.h
)

include(${SdkRootDirPath}/${board_root}/${board}/wireless_examples/ieee-802.15.4/nbu_dyn/reconfig.cmake)

mcux_add_macro(
    PHY_15_4_ENABLED=1
    FSL_RTOS_THREADX=1
    gAspCapability_d=1
    gAspNoHWParameters_c=1
    gBleBondIdentityHeaderSize_c=56
    gUseHciTransportUpward_d=1
    HCI_FREE_RxBuffer=0
    HDI_MODE=${HDI_MODE}
    MULTICORE_APP=1
    RF_OSC_26MHZ=0
    TEST_BUILD_COEX=0
    FFU_DEVICE_LIMIT_VISIBILITY=1
    FPGA_TARGET=${FPGA_TARGET}
    PHY_15_4_LOW_POWER_ENABLED=1
    TX_INCLUDE_USER_DEFINE_FILE
    CTX_SCHED
    MAC_ENABLED
    PHY_LP_TMR
)

mcux_add_configuration(
# define to force initialization of sqram_rldat_region after reset
# only required for GCC as IAR already initialize correctly this region
    TOOLCHAINS armgcc
    AS "-DINIT_BLE_RL_DAT"
)

mcux_add_iar_configuration(
    TARGETS debug
    AS "-r"
    CC "-On --debug --no_cse --no_unroll --no_inline --no_code_motion --no_tbaa --no_clustering --no_scheduling"
    CX "--debug --no_cse --no_unroll --no_code_motion --no_tbaa --no_clustering --no_scheduling"
    LD "--semihosting"
)

mcux_add_iar_configuration(
    TARGETS  release
    CC "-Oh"
    CX "-Oh"
)

mcux_add_armgcc_configuration(
    TARGETS debug
    AS "-g"
    CC "-g -Og"
    CX "-g -Og"
    LD "-g"
)

mcux_add_armgcc_configuration(
    TARGETS release
    CC "-Os"
    CX "-Os"
)

# WAR as 15.4 doesn't use the framework's SecLib implementation
mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/framework/services
    SOURCES
        SecLib_RNG/SecLib.c
)

# =============================================================================
# Build Timestamp
# =============================================================================
string(TIMESTAMP BUILD_DATE "%Y/%m/%d")
string(TIMESTAMP BUILD_TIME "%H:%M:%S")
# =============================================================================
# Manifest Repository Commit SHA
# =============================================================================
# Retrieve the short SHA from the west manifest repository.
# Falls back to "unknown"/"none" if west command fails or not in a west workspace.
# =============================================================================
execute_process(
  COMMAND west forall manifest -c "git rev-parse --short HEAD"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  OUTPUT_VARIABLE GITCOMMIT_RAW
  OUTPUT_STRIP_TRAILING_WHITESPACE
  ERROR_QUIET
  RESULT_VARIABLE WEST_RESULT
)
if(WEST_RESULT EQUAL 0 AND GITCOMMIT_RAW)
  # Extract SHA: everything after the last colon
  string(REGEX REPLACE ".*:" "" GITCOMMIT "${GITCOMMIT_RAW}")
  string(STRIP "${GITCOMMIT}" GITCOMMIT)
  if(GITCOMMIT MATCHES "^[0-9a-fA-F]+$")
    # Valid SHA - keep GITCOMMIT as is
  else()
    set(GITCOMMIT "unknown")
  endif()
else()
  set(GITCOMMIT "none")
endif()
# =============================================================================
# Build User
# =============================================================================
set(BUILD_USER "unknown")
# =============================================================================
# Pass individual components
# =============================================================================
mcux_add_macro(COMMIT_INFO=\\\"${GITCOMMIT}\\\")
mcux_add_macro(BUILD_USER=\\\"${BUILD_USER}\\\")
mcux_add_macro(BUILD_DATE=\\\"${BUILD_DATE}\\\")
mcux_add_macro(BUILD_TIME=\\\"${BUILD_TIME}\\\")
# =============================================================================
# Build Output
# =============================================================================
message(STATUS "Build Date: ${BUILD_DATE} ${BUILD_TIME}")
message(STATUS "Manifest Commit: ${GITCOMMIT}")
