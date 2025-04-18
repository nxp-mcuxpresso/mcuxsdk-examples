# Copyright 2025 NXP
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

mcux_add_macro(
    PHY_15_4_ENABLED=1
    FSL_RTOS_THREADX=1
    gAspCapability_d=1
    gAspNoHWParameters_c=1
    gBleBondIdentityHeaderSize_c=56
    gUseHciTransportUpward_d=1
    HCI_FREE_RxBuffer=0
    HDI_MODE=0
    MULTICORE_APP=1
    RF_OSC_26MHZ=0
    TEST_BUILD_COEX=0
    FFU_DEVICE_LIMIT_VISIBILITY=1
    FPGA_TARGET=0
    PHY_15_4_LOW_POWER_ENABLED=1
    CTX_SCHED
    TX_INCLUDE_USER_DEFINE_FILE
    PHY_LP_TMR
)

mcux_add_iar_configuration(
    TARGETS  release
    CC "     -Oh "
)
mcux_add_iar_configuration(
    TARGETS  debug
    CC "     --debug "
)

mcux_add_iar_configuration(
    TARGETS  debug
    LD "     --semihosting "
)

# WAR as 15.4 doesn't use the framework's SecLib implementation
mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/framework/services
    SOURCES
        SecLib_RNG/SecLib.c
)
