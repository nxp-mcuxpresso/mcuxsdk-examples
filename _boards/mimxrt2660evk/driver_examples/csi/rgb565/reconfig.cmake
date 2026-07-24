mcux_add_macro(
    CC "-DDEMO_BUFFER_FIXED_ADDRESS=1"
)

# ram + xspi_nor need ~764 KB NCACHE for camera framebuffer -- private links.
# psram / psram_txt / xspi_nor_psram use public links (32 MB / 16 MB NCACHE).

# armgcc: ram
mcux_remove_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_ram.ld
)
mcux_add_armgcc_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/driver_examples/csi/linker/MIMXRT2663_csi_ram.ld
)

# armgcc: xspi_nor
mcux_remove_armgcc_linker_script(
    TARGETS xspi_nor_debug xspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_xspi_nor.ld
)
mcux_add_armgcc_linker_script(
    TARGETS xspi_nor_debug xspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/driver_examples/csi/linker/MIMXRT2663_csi_xspi_nor.ld
)

# iar: ram
mcux_remove_iar_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_ram.icf
)
mcux_add_iar_linker_script(
    TARGETS debug release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/driver_examples/csi/linker/MIMXRT2663_csi_ram.icf
)

# iar: xspi_nor
mcux_remove_iar_linker_script(
    TARGETS xspi_nor_debug xspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_xspi_nor.icf
)
mcux_add_iar_linker_script(
    TARGETS xspi_nor_debug xspi_nor_release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/driver_examples/csi/linker/MIMXRT2663_csi_xspi_nor.icf
)
