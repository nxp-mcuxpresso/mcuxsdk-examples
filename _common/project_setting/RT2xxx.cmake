include(${SdkRootDirPath}/examples/_common/project_setting/arm_common.cmake)

if (CONFIG_MCUX_PRJSEG_module.board.suite)

    mcux_add_configuration(
        CC "-DXIP_EXTERNAL_FLASH=1"
        CX "-DXIP_EXTERNAL_FLASH=1"
    )

    mcux_add_configuration(
        TARGETS psram_debug                  psram_release
                psram_txt_debug              psram_txt_release
                xspi_nor_psram_debug         xspi_nor_psram_release
        CC "-DUSE_PSRAM=1"
        CX "-DUSE_PSRAM=1"
    )
endif()
