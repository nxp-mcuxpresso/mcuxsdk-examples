# pmic support
if(CONFIG_MCUX_PRJSEG_module.pmic_support)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}
        SOURCES pmic_support.c
                pmic_support.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}
        INCLUDES ./
    )
endif()
