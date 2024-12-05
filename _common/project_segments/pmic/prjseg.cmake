# pmic support
if(CONFIG_MCUX_PRJSEG_module.pmic_support)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/${board_root}/${board}
        SOURCES pmic_support.c
                pmic_support.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/${board_root}/${board}
        INCLUDES ./
    )
endif()
