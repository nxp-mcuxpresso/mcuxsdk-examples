include(${SdkRootDirPath}/arch/arm/target/ram.cmake)
include(${SdkRootDirPath}/arch/arm/target/flexspi_nor.cmake)
include(${SdkRootDirPath}/arch/arm/target/flexspi_nor_sdram.cmake)
include(${SdkRootDirPath}/arch/arm/target/sdram.cmake)
include(${SdkRootDirPath}/arch/arm/target/sdram_txt.cmake)
mcux_add_cmakelists(${CMAKE_CURRENT_LIST_DIR}/xip)
include(${SdkRootDirPath}/examples/_common/project_setting/RT1xxx.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/lvgl/prjseg.cmake)
include(${SdkRootDirPath}/examples/lvgl_examples/lvgl_sdk/lvgl_support/rt102x_lpspi/lvgl_support.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)

if (CONFIG_MCUX_PRJSEG_module.board.suite)
    mcux_add_configuration(
        TARGETS sdram_debug              sdram_release
                sdram_txt_debug          sdram_txt_release
                flexspi_nor_sdram_debug  flexspi_nor_sdram_release
        CC "-DSKIP_SYSCLK_INIT=1"
    )

    mcux_add_configuration(
        TARGETS sdram_debug              sdram_release
                sdram_txt_debug          sdram_txt_release
                flexspi_nor_sdram_debug  flexspi_nor_sdram_release
        CC "-DDATA_SECTION_IS_CACHEABLE=1"
    )

endif()
