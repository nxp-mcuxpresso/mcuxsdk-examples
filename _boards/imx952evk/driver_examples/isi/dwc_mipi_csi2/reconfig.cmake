
mcux_add_configuration(
	CC "-DSDK_I2C_BASED_COMPONENT_USED=1 -DBOARD_USE_ADP5585=1 -DBOARD_USE_PCAL6408=1 -DAPP_USE_DUAL_LVDS_AS_DEFAULT_AS_WORKAROUND=1"
)

mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/i.MX/i.MX952/MIMX95294/gcc/MIMX95294xxxN_cm7_ram.ld
)

mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/imx952evk/driver_examples/isi/dwc_mipi_csi2/linker/MIMX95294_cm7_isi_ram.ld
)

mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/i.MX/i.MX952/MIMX95294/iar/MIMX95294xxxN_cm7_ram.icf
)

mcux_add_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER ${board_root}/imx952evk/driver_examples/isi/dwc_mipi_csi2/linker/MIMX95294_cm7_isi_ram.icf
)

# include ISI example file
include(${SdkRootDirPath}/${board_root}/imx952evk/driver_examples/isi/reconfig.cmake)
