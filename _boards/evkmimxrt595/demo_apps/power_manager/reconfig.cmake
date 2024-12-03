
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/demo_apps/power_manager/power_manager.c
            examples/_boards/${board}/pmic_support.c
            examples/_boards/${board}/pmic_support.h
)
