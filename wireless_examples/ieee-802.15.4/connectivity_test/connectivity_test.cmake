# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    PREINCLUDE TRUE
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/ieee-802.15.4
    SOURCES
            ${NXP_IEEE_802_15_4_CONN_TEST_PREINCLUDE}/app_preinclude.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/ieee-802.15.4
    INCLUDES
            ${NXP_IEEE_802_15_4_CONN_TEST_PREINCLUDE}
            ieee_802_15_4/examples/connectivity_test
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}/middleware/wireless/ieee-802.15.4
    SOURCES
            ${NXP_IEEE_802_15_4_CONN_TEST_PREINCLUDE}/app_preinclude.h
            ieee_802_15_4/examples/connectivity_test/*.c
            ieee_802_15_4/examples/connectivity_test/*.h
            smac/interface/*.h
            smac/common/*.h
            smac/source/*.c
            smac/source/*.h
            ieee_802_15_4/examples/connectivity_test/readme.txt
            docs/connectivity_test/CTACLIUG.pdf
)

mcux_add_configuration(
    CC "-DgConnTestUsePhyTimers_c=0\
        -DUSE_NBU=${NXP_IEEE_802_15_4_CONN_TEST_USE_NBU}\
        -DgMemManagerLightExtendHeapAreaUsage=1\
        -DENABLE_RAM_VECTOR_TABLE=1\
        -DHAL_RPMSG_SELECT_ROLE=0\
        -DRPMSG_ADAPTER_NON_BLOCKING_MODE=1\
        -DSERIAL_MANAGER_NON_BLOCKING_MODE=1\
        -DHAL_FLASH_ROMAPI_DRIVER=1\
        -DHDI_MODE=0\
        -DKW45_FGPA=0\
        -DTM_ENABLE_TIME_STAMP=1\
        -DSERIAL_PORT_TYPE_UART=1\
        -DRF_OSC_26MHZ=0\
        -DTEST_BUILD_COEX=0\
        -DARM_MATH_CM3=1\
        -DMCUXPRESSO_SDK\
        -DHWINIT_DEBUG_DTEST=0\
        -DgAppUseSerialManager_c"
)
