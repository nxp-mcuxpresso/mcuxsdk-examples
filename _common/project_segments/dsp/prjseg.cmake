# dsp support
if(CONFIG_MCUX_PRJSEG_module.dsp_support)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples
        SOURCES dsp_support.c
                dsp_support.h
    )

    mcux_add_include(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples
        INCLUDES ./
    )
endif()

# incbin
if(CONFIG_MCUX_PRJSEG_module.incbin)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples
        SOURCES incbin.S
        TOOLCHAINS mdk armgcc mcux
    )
endif()

# incbin_for_core
if(CONFIG_MCUX_PRJSEG_module.incbin_for_core)
    mcux_add_source(
        BASE_PATH ${SdkRootDirPath}/examples/_boards/${board}/dsp_examples
        SOURCES incbin_${core_id}.S
        TOOLCHAINS mdk armgcc mcux
    )
endif()
