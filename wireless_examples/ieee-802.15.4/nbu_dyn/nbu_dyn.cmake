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
