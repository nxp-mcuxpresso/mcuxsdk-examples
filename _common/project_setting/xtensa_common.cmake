if (CONFIG_MCUX_PRJSEG_config.xtensa.shared)
    mcux_add_xtensa_configuration(
        TARGETS debug
        AS "-DDEBUG"
        CC "-DDEBUG"
        CX "-DDEBUG"
        LD "-Wl,--orphan-handling=place -mlsp=${XTENSA_LSP_DEBUG}"
    )

    mcux_add_xtensa_configuration(
        TARGETS release
        AS "-DNDEBUG"
        CC "-DNDEBUG"
        CX "-DNDEBUG"
        LD "-Wl,--orphan-handling=place -mlsp=${XTENSA_LSP_RELEASE}"
    )
endif()
include(${SdkRootDirPath}/examples/_common/project_setting/common.cmake)
