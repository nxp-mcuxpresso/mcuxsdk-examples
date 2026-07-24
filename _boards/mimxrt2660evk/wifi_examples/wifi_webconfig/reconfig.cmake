
#
# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
        examples/_boards/${board}/wifi_bt_config.c
        examples/_boards/${board}/wifi_bt_config.h
        examples/_boards/${board}/sdmmc_config.c
        examples/_boards/${board}/sdmmc_config.h
        examples/_boards/${board}/wifi_examples/common/hardware_init.c
        examples/_boards/${board}/wifi_examples/common/app.h
)
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES
        examples/_boards/${board}
        examples/_boards/${board}/wifi_examples/common
)

# Keep the core-added WiFi-scoped pin_mux (defines BOARD_InitUSDHC1Pins); drop board-common instead.
mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/common/pin_mux/pin_mux.c
            ${board_root}/${board}/common/pin_mux/pin_mux.h
)
mcux_project_remove_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/common/pin_mux
)

mcux_add_macro(
    TOOLCHAINS iar
    CC "-DFSL_DRIVER_TRANSFER_DOUBLE_WEAK_IRQ"
)

mcux_add_iar_configuration(
	CC "--dlib_config full\
		--no_inline"
	CX "--no_clustering"
	LD "--semihosting"
)

mcux_add_macro(
    CC "-DBOARD_USE_PCAL6524=1\
        -DBOARD_USE_PCA9555=1\
        -DSDK_I2C_BASED_COMPONENT_USED=1\
	-DconfigENABLE_MVE=1\
	-DconfigENABLE_FPU=1\
	-DconfigENABLE_TRUSTZONE=0\
	-DconfigRUN_FREERTOS_SECURE_ONLY=1 -DBOARD_TRDC_ALL_MASTER_TO_PREVELEGE_DOMAIN=1 -DconfigENABLE_MPU=0"
)

# Add or remove Linker File Configurations
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_release xspi_nor_psram_debug
    LINKER devices/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_xspi_nor_psram.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_release xspi_nor_psram_debug
    LINKER devices/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_xspi_nor_psram.icf
)
# Add or remove Linker File Configurations
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_release xspi_nor_psram_debug
    LINKER examples/_boards/${board}/wifi_examples/common/linker/MIMXRT2663xxxxx_xspi_nor_psram.ld
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_release xspi_nor_psram_debug
    LINKER examples/_boards/${board}/wifi_examples/common/linker/MIMXRT2663xxxxx_xspi_nor_psram.icf
)
