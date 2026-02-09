# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_configuration(
        CC "-DSDK_I2C_BASED_COMPONENT_USED=1 -DBOARD_USE_ADP5585=1 -DRM67199_ENABLE=1"
)
mcux_add_configuration(
        CC "-include ${SdkRootDirPath}/${board_root}/${board}/demo_apps/isp_ccl/${core_id}/preinclude.h"
)
mcux_add_configuration(
        CC "-DIMX95"
)
if (DEFINED CONFIG_MCUX_MISC_middleware.ispsdk.socrev)
        message(STATUS "IMX95 SOC revision: ${CONFIG_MCUX_MISC_middleware.ispsdk.socrev}")
        mcux_add_configuration(
                CC "-D${CONFIG_MCUX_MISC_middleware.ispsdk.socrev}"
        )
else()
        message(WARNING "IMX95 SOC revision not defined, defaulting to B0.")
        mcux_add_configuration(
                CC "-DB0"
        )
endif()

mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/i.MX/i.MX95/MIMX9596/gcc/MIMX9596xxxxN_cm7_ram.ld
)

mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/${board}/demo_apps/isp_ccl/cm7/linker/MIMX9596_cm7_ram.ld
)

mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/i.MX/i.MX95/MIMX9596/iar/MIMX9596xxxxN_cm7_ram.icf
)

mcux_add_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/${board}/demo_apps/isp_ccl/cm7/linker/MIMX9596_cm7_ram.icf
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
    ${board_root}/${board}/display_support.h
    ${board_root}/${board}/display_support.c
    ${board_root}/${board}/demo_apps/isp_ccl/cm7/board_isp.c
)

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.usb_cdc_acm)
        mcux_add_macro(
                CC "-DDATA_SECTION_IS_CACHEABLE=0\
                -DSDK_I2C_BASED_COMPONENT_USED=1\
                -DBOARD_USE_PCAL6524=1\
                -DUSB_DEVICE_CONFIG_EHCI=1"
                TARGETS
                "debug"
                "release"
                TOOLCHAINS
                "armgcc"
        )
endif()

if(CONFIG_MCUX_COMPONENT_middleware.ispsdk.eth)
        mcux_add_macro(
                CC "-DCONFIG_ISPSDK_ETH \
                -DSDK_I2C_BASED_COMPONENT_USED=1 \
                -DBOARD_USE_PCAL6524=1 \
                -DBOARD_USE_PCAL6408=1 \
                -DUSE_RTOS=1"
        )

        mcux_add_source(
                BASE_PATH ${SdkRootDirPath}
                SOURCES ${board_root}/${board}/lwip_examples/lwip_netc_port.h
        )

        mcux_add_include(
                BASE_PATH ${SdkRootDirPath}
                INCLUDES ${board_root}/${board}/lwip_examples
        )

        mcux_add_linker_symbol(
                SYMBOLS "__stack_size__=2048\
                        __heap_size__=35840"
        )
endif()