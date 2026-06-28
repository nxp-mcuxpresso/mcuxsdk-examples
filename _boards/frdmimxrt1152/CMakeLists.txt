include(${SdkRootDirPath}/arch/arm/target/ram.cmake)
include(${SdkRootDirPath}/arch/arm/target/flexspi_nor.cmake)
include(${SdkRootDirPath}/arch/arm/target/flexspi_nor_hyperram.cmake)
include(${SdkRootDirPath}/arch/arm/target/hyperram.cmake)
include(${SdkRootDirPath}/arch/arm/target/hyperram_txt.cmake)
mcux_add_cmakelists(${CMAKE_CURRENT_LIST_DIR}/xip)
include(${SdkRootDirPath}/examples/_common/project_setting/RT1xxx.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/display_support/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/vglite_support/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/lvgl/prjseg.cmake)
include(${SdkRootDirPath}/examples/lvgl_examples/lvgl_sdk/lvgl_support/rt11xx/lvgl_support.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)

if (CONFIG_MCUX_PRJSEG_module.board.suite)
    mcux_add_configuration(
        TARGETS hyperram_debug              hyperram_release
                flexspi_nor_hyperram_debug  flexspi_nor_hyperram_release
                hyperram_txt_debug          hyperram_txt_release
        CC "-DSKIP_SYSCLK_INIT=1"
    )

    mcux_add_configuration(
        TARGETS hyperram_debug              hyperram_release
                flexspi_nor_hyperram_debug  flexspi_nor_hyperram_release
                hyperram_txt_debug          hyperram_txt_release
        CC "-DDATA_SECTION_IS_CACHEABLE=1"
    )

    mcux_add_iar_configuration(
        TARGETS flexspi_nor_debug        flexspi_nor_release
                flexspi_nor_hyperram_debug  flexspi_nor_hyperram_release
        LD "--config_def=__use_flash64MB__=1"
    )

    mcux_add_mdk_configuration(
        TARGETS flexspi_nor_debug        flexspi_nor_release
                flexspi_nor_hyperram_debug  flexspi_nor_hyperram_release
        LD "--predefine=\"-D__use_flash64MB__=1\""
    )

    mcux_add_armgcc_configuration(
        TARGETS flexspi_nor_debug        flexspi_nor_release
                flexspi_nor_hyperram_debug  flexspi_nor_hyperram_release
        LD "-Xlinker --defsym=__use_flash64MB__=1"
    )

endif()
