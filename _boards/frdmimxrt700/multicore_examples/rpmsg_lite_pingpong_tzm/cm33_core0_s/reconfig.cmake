#
# Copyright 2024, 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${project_board_port_path}/hardware_init.c
            ${project_board_port_path}/app.h
)
mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${project_board_port_path})

mcux_add_source(
    SOURCES tzm_config.c tzm_config.h
)

mcux_add_include(
    INCLUDES .
)

mcux_add_include(
        TOOLCHAINS mdk armgcc
        BASE_PATH ${APPLICATION_BINARY_DIR}
        INCLUDES ../rpmsg_lite_pingpong_tzm_secondary_core_ns/${CONFIG_TOOLCHAIN}/
)

mcux_add_mdk_configuration(
        CC "-DCORE1_IMAGE_COPY_TO_RAM"
)

mcux_add_iar_configuration(
        CC "-DCORE1_IMAGE_COPY_TO_RAM"
)

mcux_add_armgcc_configuration(
        CC "-DCORE1_IMAGE_COPY_TO_RAM"
)

# Remove no_se from IAR FLags
mcux_remove_iar_configuration(
        AS "--cpu=cortex-m33.no_se"
        CC "--cpu=cortex-m33.no_se"
        CX "--cpu=cortex-m33.no_se"
        LD "--cpu=cortex-m33.no_se"
)

# And then this will add Trust Zone Enable clicky button checked in IAR GUI Project
mcux_add_iar_configuration(
        AS "--cpu=cortex-m33"
        CC "--cpu=cortex-m33"
        CX "--cpu=cortex-m33"
        LD "--cpu=cortex-m33"
)

mcux_add_iar_configuration(
    LD "--image_input=${APPLICATION_BINARY_DIR}/../rpmsg_lite_pingpong_tzm_secondary_core_ns/iar/core1_ns_image.bin,_core1_ns_image,__core1_ns_image,4\
        --keep=_core1_ns_image"
)

mcux_add_mdk_configuration(
    LD "--keep=*(*core1_ns_code)"
)

mcux_remove_iar_linker_script(
        TARGETS flash_debug flash_release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
)

mcux_remove_armgcc_linker_script(
        TARGETS flash_debug flash_release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)

mcux_remove_mdk_linker_script(
        TARGETS flash_debug flash_release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
)

mcux_add_iar_linker_script(
        TARGETS flash_debug flash_release
        LINKER ../linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_s.icf
)

mcux_add_armgcc_linker_script(
        TARGETS flash_debug flash_release
        LINKER ../linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_s.ld
)

mcux_add_mdk_linker_script(
        TARGETS flash_debug flash_release
        LINKER ../linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash_s.scf
)
