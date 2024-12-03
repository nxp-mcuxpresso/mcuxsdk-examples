if (CONFIG_MCUX_PRJSEG_config.dsp56800.shared)
    mcux_add_codewarrior_configuration(
        CC "-D__DSC__ -D__CW__"
    )
endif()
include(${SdkRootDirPath}/examples/_common/project_setting/common.cmake)
