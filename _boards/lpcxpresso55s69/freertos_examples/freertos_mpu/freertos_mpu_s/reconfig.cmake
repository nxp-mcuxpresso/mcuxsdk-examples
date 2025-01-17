#
# Copyright 2024 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_configuration(
    AS "-DDEMO_CODE_START_NS=65536"
    CC "-DDEMO_CODE_START_NS=65536 "
    CX "-DDEMO_CODE_START_NS=65536"
    )

mcux_add_source(
    SOURCES nsc_functions.c
            nsc_functions.h
            tzm_config.c
            tzm_config.h
)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/freertos_config_board/${core_id}/FreeRTOSConfigBoard.h
)

mcux_add_source(
    SOURCES hardware_init.c
            app.h
            pin_mux.c
            pin_mux.h
)
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/${board}/freertos_config_board/${core_id}/
)

mcux_add_include(
    INCLUDES ./
)

mcux_add_custom_command(
    BUILD_EVENT PRE_BUILD
    TOOLCHAINS armgcc
    BUILD_COMMAND ${CMAKE_COMMAND} -E make_directory ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}
)

mcux_add_armgcc_configuration(
    LD "-Wl,--cmse-implib"
    LD "-Wl,--out-implib=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/${board}_freertos_mpu_s_CMSE_lib.o"
)

mcux_add_mdk_configuration(
    LD "--import-cmse-lib-out=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/${board}_freertos_mpu_s_CMSE_lib.o"
)

mcux_add_iar_configuration(
    LD "--import_cmse_lib_out=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/${board}_freertos_mpu_s_CMSE_lib.o"
)

mcux_convert_binary(
        TOOLCHAINS armgcc mdk iar
        BINARY ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/${board}_freertos_mpu_s_CMSE_lib.bin
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/LPC/LPC5500/LPC55S69/iar/LPC55S69_cm33_core0_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/LPC/LPC5500/LPC55S69/arm/LPC55S69_cm33_core0_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/LPC/LPC5500/LPC55S69/gcc/LPC55S69_cm33_core0_flash.ld
)



mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/LPC/LPC5500/LPC55S69/iar/LPC55S69_cm33_core0_flash_s.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/LPC/LPC5500/LPC55S69/gcc/LPC55S69_cm33_core0_flash_s.ld
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/LPC/LPC5500/LPC55S69/arm/LPC55S69_cm33_core0_flash_s.scf
)
