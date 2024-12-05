
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/power_manager/power_manager.c
            ${board_root}/${board}/pmic_support.c
            ${board_root}/${board}/pmic_support.h
            ${board_root}/${board}/FlashIMXRT600_EVK_FLEXSPI_B_QSPI.board
)
