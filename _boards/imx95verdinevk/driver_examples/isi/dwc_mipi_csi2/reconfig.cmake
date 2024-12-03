
mcux_add_configuration(
	CC "-DSDK_I2C_BASED_COMPONENT_USED=1 -DBOARD_USE_ADP5585=1 -DBOARD_USE_PCAL6408=1 -DBOARD_USE_PCAL6524=1 -DRM67199_ENABLE=1"
)

mcux_remove_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/i.MX/i.MX95/MIMX9596/gcc/MIMX9596xxxxN_cm7_ram.ld
)

mcux_add_armgcc_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER examples/_boards/imx95verdinevk/driver_examples/isi/dwc_mipi_csi2/linker/MIMX9596_cm7_isi_ram.ld
)

mcux_remove_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER devices/i.MX/i.MX95/MIMX9596/iar/MIMX9596xxxxN_cm7_ram.icf
)

mcux_add_iar_linker_script(
        TARGETS debug release
        BASE_PATH ${SdkRootDirPath}
        LINKER examples/_boards/imx95verdinevk/driver_examples/isi/dwc_mipi_csi2/linker/MIMX9596_cm7_isi_ram.icf
)

# include ISI example file
include(${SdkRootDirPath}/examples/_boards/imx95verdinevk/driver_examples/isi/reconfig.cmake)
