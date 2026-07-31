# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# The on-board APS512XXN is an Xccela (OPI/HPI) PSRAM, not HyperBus: swap the
# shared HyperBus ops implementation for the board-specific one.

mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/xspi/psram/polling_transfer/xspi_psram_ops.c
)
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/xspi/psram/polling_transfer/xspi_psram_ops.c
)

# PSRAM-resident targets use example-private linker scripts that
#  - carve a 32 KB linker-untouched test window (__PSRAM_TEST_START__) out of
#    the NCACHE tail, and
#  - pin the XSPI driver, the shared ops, and the board hardware_init code
#    and data into ITCM/DTCM (flash-driver style object placement), so the
#    example can re-program the XSPI1 controller it is (partially) resident
#    behind.
# See ../linker/*.ld and hardware_init.c.

# armgcc: psram (text in flash, data in PSRAM)
mcux_remove_armgcc_linker_script(
    TARGETS psram_debug psram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_psram.ld
)
mcux_add_armgcc_linker_script(
    TARGETS psram_debug psram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/driver_examples/xspi/psram/linker/MIMXRT2663_xspi_psram_psram.ld
)

# armgcc: psram_txt (text and data in PSRAM)
mcux_remove_armgcc_linker_script(
    TARGETS psram_txt_debug psram_txt_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_psram_txt.ld
)
mcux_add_armgcc_linker_script(
    TARGETS psram_txt_debug psram_txt_release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/driver_examples/xspi/psram/linker/MIMXRT2663_xspi_psram_psram_txt.ld
)

# armgcc: xspi_nor_psram (text XIP in flash, data in PSRAM)
mcux_remove_armgcc_linker_script(
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_xspi_nor_psram.ld
)
mcux_add_armgcc_linker_script(
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/driver_examples/xspi/psram/linker/MIMXRT2663_xspi_psram_xspi_nor_psram.ld
)

# iar: same customization (.icf mirrors of the .ld files above)
mcux_remove_iar_linker_script(
    TARGETS psram_debug psram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_psram.icf
)
mcux_add_iar_linker_script(
    TARGETS psram_debug psram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/driver_examples/xspi/psram/linker/MIMXRT2663_xspi_psram_psram.icf
)
mcux_remove_iar_linker_script(
    TARGETS psram_txt_debug psram_txt_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_psram_txt.icf
)
mcux_add_iar_linker_script(
    TARGETS psram_txt_debug psram_txt_release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/driver_examples/xspi/psram/linker/MIMXRT2663_xspi_psram_psram_txt.icf
)
mcux_remove_iar_linker_script(
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_xspi_nor_psram.icf
)
mcux_add_iar_linker_script(
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/${board}/driver_examples/xspi/psram/linker/MIMXRT2663_xspi_psram_xspi_nor_psram.icf
)
