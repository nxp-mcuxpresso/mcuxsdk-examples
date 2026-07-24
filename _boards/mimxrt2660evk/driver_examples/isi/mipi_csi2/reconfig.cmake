# Copyright 2026 NXP
# SPDX-License-Identifier: BSD-3-Clause

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/isi_board.c
            ${board_root}/${board}/isi_example.h
            ${board_root}/${board}/driver_examples/isi/mipi_csi2/app_display.c
            ${board_root}/${board}/driver_examples/isi/mipi_csi2/app_display.h
            ${board_root}/${board}/display_support.c
            ${board_root}/${board}/display_support.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/driver_examples/isi/mipi_csi2
             ${board_root}/${board}/driver_examples/isi
             ${board_root}/${board}
             examples/drivers/mipi_csi
)

# ram + xspi_nor need ~600 KB NCACHE for camera framebuffer -- private links.
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
    LINKER examples/_boards/${board}/driver_examples/isi/mipi_csi2/linker/MIMXRT2663_isi_ram.ld
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
    LINKER examples/_boards/${board}/driver_examples/isi/mipi_csi2/linker/MIMXRT2663_isi_xspi_nor.ld
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
    LINKER examples/_boards/${board}/driver_examples/isi/mipi_csi2/linker/MIMXRT2663_isi_ram.icf
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
    LINKER examples/_boards/${board}/driver_examples/isi/mipi_csi2/linker/MIMXRT2663_isi_xspi_nor.icf
)
