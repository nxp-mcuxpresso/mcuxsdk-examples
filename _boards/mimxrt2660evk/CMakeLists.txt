#
# Adding pin_mux_fpga, which is selected by prj.conf
#

include(${SdkRootDirPath}/arch/arm/target/ram.cmake)
include(${SdkRootDirPath}/arch/arm/target/xspi_nor.cmake)
include(${SdkRootDirPath}/arch/arm/target/xspi_nor_psram.cmake)
include(${SdkRootDirPath}/arch/arm/target/psram.cmake)
include(${SdkRootDirPath}/arch/arm/target/psram_txt.cmake)
include(${SdkRootDirPath}/arch/arm/target/RT2xxx/ram_only.cmake)
include(${SdkRootDirPath}/arch/arm/target/RT2xxx/psram_only.cmake)
mcux_add_cmakelists(${CMAKE_CURRENT_LIST_DIR}/xip)
include(${SdkRootDirPath}/examples/_common/project_setting/RT2xxx.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/display_support/prjseg.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/lvgl/prjseg.cmake)
include(${SdkRootDirPath}/examples/lvgl_examples/lvgl_sdk/lvgl_support/rt2xxx/lvgl_support.cmake)
include(${SdkRootDirPath}/examples/_common/project_segments/freertos/examples/tickless/prjseg.cmake)

if (CONFIG_MCUX_PRJSEG_module.board.suite)

    mcux_add_configuration(
        TARGETS debug            release
                psram_debug      psram_release
                psram_txt_debug  psram_txt_release
                psram_only_debug psram_only_release
        CC "-DENABLE_RAM_VECTOR_TABLE"
        CX "-DENABLE_RAM_VECTOR_TABLE"
    )
    mcux_add_iar_configuration(
        TARGETS debug            release
                psram_debug      psram_release
                psram_txt_debug  psram_txt_release
                psram_only_debug psram_only_release
        LD "--config_def=__ram_vector_table__=1"
    )
    mcux_add_mdk_configuration(
        TARGETS debug            release
                psram_debug      psram_release
                psram_txt_debug  psram_txt_release
                psram_only_debug psram_only_release
        LD "--predefine=\"-D__ram_vector_table__=1\""
    )
    mcux_add_armgcc_configuration(
        TARGETS debug            release
                psram_debug      psram_release
                psram_txt_debug  psram_txt_release
                psram_only_debug psram_only_release
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
        CC "-DUSE_PSRAM_W958D6=1"
        CX "-DUSE_PSRAM_W958D6=1"
    )

    mcux_add_configuration(
        CC "-DUSE_FLASH_W35T51NW=0"
        CX "-DUSE_FLASH_W35T51NW=0"
    )
    mcux_add_configuration(
        CC "-DUSE_FLASH_W25H512NW=0"
        CX "-DUSE_FLASH_W25H512NW=0"
    )
endif()

if(CONFIG_MCUX_PRJSEG_module.board.boardfile)
    mcux_add_source(
        SOURCES
        gpio_handle.c
        gpio_handle.h
    )

    mcux_add_include(
        INCLUDES
        .
    )
endif()

if (CONFIG_MCUX_COMPONENT_driver.pcal6524)
    mcux_add_macro(CC "-DBOARD_USE_PCAL6524=1 -DSDK_I2C_BASED_COMPONENT_USED=1 -DBOARD_USE_GPIO0_HANDLE=1")
endif()
if (CONFIG_MCUX_COMPONENT_driver.pca9555)
    mcux_add_macro(CC "-DBOARD_USE_PCA9555=1 -DSDK_I2C_BASED_COMPONENT_USED=1 -DBOARD_USE_GPIO1_HANDLE=1")
endif()
if (CONFIG_MCUX_BOARD_USE_PF9453)
    mcux_add_macro(CC "-DBOARD_USE_PF9453=1 -DSDK_I2C_BASED_COMPONENT_USED=1")
endif()
