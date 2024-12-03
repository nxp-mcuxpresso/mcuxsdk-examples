if (CONFIG_MCUX_PRJSEG_config.arm.shared)
    mcux_add_iar_configuration(
        LD "--entry Reset_Handler"
    )

    mcux_add_armgcc_configuration(
        AS "-D__STARTUP_CLEAR_BSS"
    )

    mcux_add_mcux_configuration(
        AS "-D__STARTUP_CLEAR_BSS"
    )

    mcux_add_configuration(
        TARGETS debug
                flash_debug
                flexspi_nor_debug
                flexspi_nor_sdram_debug
                flexspi_nor_hyperram_debug
                sdram_debug
                sdram_txt_debug
                hyperram_debug
                hyperram_txt_debug
                ram_0x1400_debug
        CC "-DDEBUG"
        CX "-DDEBUG"
        TOOLCHAINS iar mdk armgcc
    )

    mcux_add_configuration(
        TARGETS release
                flash_release
                flexspi_nor_release
                flexspi_nor_sdram_release
                flexspi_nor_hyperram_release
                sdram_release
                sdram_txt_release
                hyperram_release
                hyperram_txt_release
                ram_0x1400_release
        CC "-DNDEBUG"
        CX "-DNDEBUG"
        TOOLCHAINS iar mdk armgcc
    )
endif()
include(${SdkRootDirPath}/examples/_common/project_setting/common.cmake)
