include(${SdkRootDirPath}/arch/arm/target/ram.cmake)
include(${SdkRootDirPath}/arch/arm/target/flexspi_nor.cmake)
include(${SdkRootDirPath}/arch/arm/target/flexspi_nor_sdram.cmake)
include(${SdkRootDirPath}/arch/arm/target/sdram.cmake)
include(${SdkRootDirPath}/arch/arm/target/sdram_txt.cmake)
include(${SdkRootDirPath}/examples/_common/project_setting/RT1xxx.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/${core_id}/setting.cmake OPTIONAL)
include(${SdkRootDirPath}/examples/_common/project_segments/display_support/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/vglite_support/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/lvgl/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)
include(${SdkRootDirPath}/examples/lvgl_examples/lvgl_sdk/lvgl_support/rt11xx/lvgl_support.cmake)

if (CONFIG_MCUX_PRJSEG_module.board.suite)
    mcux_add_configuration(
        TARGETS sdram_debug              sdram_release
                flexspi_nor_sdram_debug  flexspi_nor_sdram_release
                sdram_txt_debug          sdram_txt_release
        CC "-DSKIP_SYSCLK_INIT=1"
    )

    mcux_add_configuration(
        TARGETS sdram_debug              sdram_release
                flexspi_nor_sdram_debug  flexspi_nor_sdram_release
                sdram_txt_debug          sdram_txt_release
        CC "-DDATA_SECTION_IS_CACHEABLE=1"
    )

endif()
