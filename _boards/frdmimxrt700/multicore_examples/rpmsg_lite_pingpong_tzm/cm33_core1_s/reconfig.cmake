#
# Copyright 2024, 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_tzm/${core_id}_s/hardware_init.c
            ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_tzm/${core_id}_s/app.h
)
mcux_add_include(BASE_PATH ${SdkRootDirPath} INCLUDES ${board_root}/${board}/multicore_examples/rpmsg_lite_pingpong_tzm/${core_id}_s)

mcux_add_source(
    SOURCES tzm_config.c tzm_config.h
)

mcux_add_include(
    INCLUDES .
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/trustzone_examples/tzm_api/tzm_api.c
            examples/trustzone_examples/tzm_api/tzm_api.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/trustzone_examples/tzm_api/
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/multicore_examples/rpmsg_lite_pingpong_tzm/veneer_table.c
            examples/multicore_examples/rpmsg_lite_pingpong_tzm/veneer_table.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/multicore_examples/rpmsg_lite_pingpong_tzm/
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
    CC "--cmse"
    LD "--import_cmse_lib_out=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/rpmsg_lite_pingpong_tzm_secondary_s_CMSE_lib.o"
)

# Create output directory for armgcc to put library in.
mcux_add_custom_command(
    BUILD_EVENT PRE_BUILD
    TOOLCHAINS armgcc
    BUILD_COMMAND ${CMAKE_COMMAND} -E make_directory ${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}
)

mcux_add_armgcc_configuration(
    CC "-mcmse"
    LD "-Wl,--cmse-implib \
        -Wl,--out-implib=${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/rpmsg_lite_pingpong_tzm_secondary_s_CMSE_lib.o"
)

mcux_add_mdk_configuration(
    CC "-mcmse"
    LD "--import-cmse-lib-out=\"${APPLICATION_BINARY_DIR}/${CONFIG_TOOLCHAIN}/rpmsg_lite_pingpong_tzm_secondary_s_CMSE_lib.o\""
)

mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.icf
)

mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
)

mcux_remove_mdk_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.scf
)

mcux_add_iar_linker_script(
        TARGETS debug release
        LINKER ../linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram_s.icf
)

mcux_add_armgcc_linker_script(
        TARGETS debug release
        LINKER ../linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram_s.ld
)

mcux_add_mdk_linker_script(
        TARGETS debug release
        LINKER ../linkscripts/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram_s.scf
)

