#
# Adding pin_mux_fpga, which is selected by prj.conf
#

include(${SdkRootDirPath}/arch/arm/target/ram.cmake)
include(${SdkRootDirPath}/arch/arm/target/xspi_nor.cmake)
include(${SdkRootDirPath}/arch/arm/target/xspi_nor_psram.cmake)
include(${SdkRootDirPath}/arch/arm/target/psram.cmake)
include(${SdkRootDirPath}/arch/arm/target/psram_txt.cmake)
mcux_add_cmakelists(${CMAKE_CURRENT_LIST_DIR}/xip)
include(${SdkRootDirPath}/examples/_common/project_setting/RT2xxx.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/display_support/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/lvgl/prjseg.cmake)
include(${SdkRootDirPath}/examples/lvgl_examples/lvgl_sdk/lvgl_support/rt2xxx/lvgl_support.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)

if (CONFIG_MCUX_PRJSEG_module.board.suite)

    mcux_add_configuration(
        TARGETS debug           release
                psram_debug     psram_release
                psram_txt_debug psram_txt_release
        CC "-DENABLE_RAM_VECTOR_TABLE=1"
        CX "-DENABLE_RAM_VECTOR_TABLE=1"
    )
    mcux_add_iar_configuration(
        TARGETS debug           release
                psram_debug     psram_release
                psram_txt_debug psram_txt_release
        LD "--config_def=__ram_vector_table__=1"
    )
    mcux_add_mdk_configuration(
        TARGETS debug           release
                psram_debug     psram_release
                psram_txt_debug psram_txt_release
        LD "--predefine=\"-D__ram_vector_table__=1\""
    )
    mcux_add_armgcc_configuration(
        TARGETS debug           release
                psram_debug     psram_release
                psram_txt_debug psram_txt_release
        LD "-Xlinker --defsym=__ram_vector_table__=1"
    )

    mcux_add_configuration(
        CC "-DPRINTF_ADVANCED_ENABLE=1\
            -DPRINTF_FLOAT_ENABLE=1"
        CX "-DPRINTF_ADVANCED_ENABLE=1\
            -DPRINTF_FLOAT_ENABLE=1"
    )

    mcux_add_iar_configuration(
        CC "--cmse"
        CX "--cmse"
    )
    mcux_add_armgcc_configuration(
        CC "-mcmse"
        CX "-mcmse"
    )
    mcux_add_mdk_configuration(
        CC "-mcmse"
        CX "-mcmse"
    )

    mcux_add_configuration(
        CC "-DUSE_PSRAM_W958D6=0"
        CX "-DUSE_PSRAM_W958D6=0"
    )

endif()

if (CONFIG_MCUX_COMPONENT_driver.pcal6524)
    mcux_add_macro(CC "-DBOARD_USE_PCAL6524=1 -DSDK_I2C_BASED_COMPONENT_USED=1")
endif()
if (CONFIG_MCUX_COMPONENT_driver.pca9555)
    mcux_add_macro(CC "-DBOARD_USE_PCA9555=1 -DSDK_I2C_BASED_COMPONENT_USED=1")
endif()
