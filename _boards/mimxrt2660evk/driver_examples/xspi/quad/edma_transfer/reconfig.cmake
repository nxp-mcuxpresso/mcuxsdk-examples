mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/driver_examples/xspi/quad/edma_transfer/xspi_quad_flash_edma_ops.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/driver_examples/xspi/quad/edma_transfer/xspi_quad_flash_edma_ops.c
)

# The example reconfigures/erases the XSPI0 boot NOR it XIPs from, so the XSPI
# reconfiguration path must run from ITCM (see the polling example for the full
# rationale). The EDMA variant additionally takes the EDMA channel-completion
# interrupt while the flash array is busy programming, so on the flash-XIP
# targets the WHOLE interrupt path must be flash-free as well: the vector table
# moves to ITCM (RAM vector table) and the board linker places fsl_edma.o,
# fsl_edma_soc.o and fsl_xspi_edma.o in ITCM next to fsl_xspi.o and the board
# ops. psram / psram_txt already run all code from ITCM / PSRAM (and already
# use a RAM vector table), so they use the stock device linker.
mcux_add_configuration(
    TARGETS xspi_nor_debug xspi_nor_release xspi_nor_psram_debug xspi_nor_psram_release
    CC "-DENABLE_RAM_VECTOR_TABLE=1"
)
mcux_add_iar_configuration(
    TARGETS xspi_nor_debug xspi_nor_release xspi_nor_psram_debug xspi_nor_psram_release
    LD "--config_def=__ram_vector_table__=1"
)
mcux_add_armgcc_configuration(
    TARGETS xspi_nor_debug xspi_nor_release xspi_nor_psram_debug xspi_nor_psram_release
    LD "-Xlinker --defsym=__ram_vector_table__=1"
)

mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_debug xspi_nor_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_xspi_nor.ld
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_debug xspi_nor_release
    LINKER ${board_root}/${board}/driver_examples/xspi/quad/edma_transfer/linker_files/MIMXRT2663xxxxx_xspi_nor.ld
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_xspi_nor_psram.ld
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    LINKER ${board_root}/${board}/driver_examples/xspi/quad/edma_transfer/linker_files/MIMXRT2663xxxxx_xspi_nor_psram.ld
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_debug psram_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_psram.ld
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_debug psram_release
    LINKER ${board_root}/${board}/driver_examples/xspi/quad/edma_transfer/linker_files/MIMXRT2663xxxxx_psram.ld
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_txt_debug psram_txt_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_psram_txt.ld
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_txt_debug psram_txt_release
    LINKER ${board_root}/${board}/driver_examples/xspi/quad/edma_transfer/linker_files/MIMXRT2663xxxxx_psram_txt.ld
)

# IAR: same intent -- place the XSPI reconfig path + EDMA interrupt path in
# ITCM (via the .icf APP_QACCESS_CODE block + initialize-by-copy).
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_debug xspi_nor_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_xspi_nor.icf
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_debug xspi_nor_release
    LINKER ${board_root}/${board}/driver_examples/xspi/quad/edma_transfer/linker_files/MIMXRT2663xxxxx_xspi_nor.icf
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_xspi_nor_psram.icf
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    LINKER ${board_root}/${board}/driver_examples/xspi/quad/edma_transfer/linker_files/MIMXRT2663xxxxx_xspi_nor_psram.icf
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_debug psram_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_psram.icf
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_debug psram_release
    LINKER ${board_root}/${board}/driver_examples/xspi/quad/edma_transfer/linker_files/MIMXRT2663xxxxx_psram.icf
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_txt_debug psram_txt_release
    LINKER ${device_root}/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_psram_txt.icf
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS psram_txt_debug psram_txt_release
    LINKER ${board_root}/${board}/driver_examples/xspi/quad/edma_transfer/linker_files/MIMXRT2663xxxxx_psram_txt.icf
)
