
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/power_manager/power_manager.c
            examples/_boards/${board}/pmic_support.c
            examples/_boards/${board}/pmic_support.h
            examples/_boards/${board}/FlashIMXRT600_EVK_FLEXSPI_B_QSPI.board
)
