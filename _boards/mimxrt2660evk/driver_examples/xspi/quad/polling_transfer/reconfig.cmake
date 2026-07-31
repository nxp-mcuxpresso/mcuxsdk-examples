mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/xspi/quad/polling_transfer/xspi_quad_flash_ops.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/xspi/quad/polling_transfer/xspi_quad_flash_ops.c
)

# The example reconfigures/erases the XSPI0 boot NOR it XIPs from, so the XSPI
# reconfiguration path must run from ITCM. That path is the XSPI driver
# (fsl_xspi.o) plus the board flash-ops (xspi_quad_flash_ops.o). NOTE: the XSPI
# driver's RAMFUNC is a no-op here (CONFIG_FLASH_DRIVER_EXECUTES_FROM_RAM=0), so
# fsl_xspi.o would otherwise stay in flash -- the board linker moves BOTH those
# objects into ITCM. Clock code stays in flash (this example does not change the
# XSPI0 clock, so there is no flash-unavailable window from a clock switch), and
# the vector table stays in flash. On the flash-XIP targets (xspi_nor,
# xspi_nor_psram) a board linker does this; psram / psram_txt already run all
# code from ITCM / PSRAM so they use the stock device linker. The one
# flash-resident call in the ops path (SDK_DelayAtLeastUs) is replaced by a
# RAM-resident xspi_quad_delay_us() in the ops source.
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_debug xspi_nor_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_xspi_nor.ld
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_debug xspi_nor_release
    LINKER ${board_root}/${board}/driver_examples/xspi/quad/polling_transfer/linker_files/MIMXRT2663xxxxx_xspi_nor.ld
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_xspi_nor_psram.ld
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    LINKER ${board_root}/${board}/driver_examples/xspi/quad/polling_transfer/linker_files/MIMXRT2663xxxxx_xspi_nor_psram.ld
)

# IAR: same intent -- place fsl_xspi.o + the board ops in ITCM (via the .icf
# APP_QACCESS_CODE block + initialize-by-copy). Self-gates on the iar toolchain.
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_debug xspi_nor_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_xspi_nor.icf
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_debug xspi_nor_release
    LINKER ${board_root}/${board}/driver_examples/xspi/quad/polling_transfer/linker_files/MIMXRT2663xxxxx_xspi_nor.icf
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_xspi_nor_psram.icf
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    LINKER ${board_root}/${board}/driver_examples/xspi/quad/polling_transfer/linker_files/MIMXRT2663xxxxx_xspi_nor_psram.icf
)
