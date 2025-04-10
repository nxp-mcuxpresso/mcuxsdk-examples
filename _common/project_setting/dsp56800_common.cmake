if (CONFIG_MCUX_PRJSEG_config.dsp56800.shared)
    mcux_add_codewarrior_configuration(
        TARGETS flash_sdm_spm_debug
                flash_sdm_lpm_debug
                flash_sdm_hpm_debug
                flash_ldm_lpm_debug
                flash_ldm_hpm_debug
        CC "-DDEBUG"
        CX "-DDEBUG"
    )

    mcux_add_codewarrior_configuration(
        TARGETS flash_sdm_spm_release
                flash_sdm_lpm_release
                flash_sdm_hpm_release
                flash_ldm_lpm_release
                flash_ldm_hpm_release
        CC "-DNDEBUG"
        CX "-DNDEBUG"
    )
    
    mcux_add_codewarrior_configuration(
        TARGETS flash_sdm_spm_debug flash_sdm_spm_release
                flash_sdm_lpm_debug flash_sdm_lpm_release
                flash_sdm_hpm_debug flash_sdm_hpm_release
        CC "-D__SDM__"
        CX "-D__SDM__"
    )
    
    mcux_add_codewarrior_configuration(
        TARGETS flash_ldm_lpm_debug flash_ldm_lpm_release
                flash_ldm_hpm_debug flash_ldm_hpm_release
        CC "-D__LDM__"
        CX "-D__LDM__"
    )
    
    mcux_add_codewarrior_configuration(
        TARGETS flash_sdm_spm_debug flash_sdm_spm_release
        CC "-D__SPM__"
        CX "-D__SPM__"
    )
    
    mcux_add_codewarrior_configuration(
        TARGETS flash_sdm_lpm_debug flash_sdm_lpm_release
                flash_ldm_lpm_debug flash_ldm_lpm_release
        CC "-D__LPM__"
        CX "-D__LPM__"
    )
    
    mcux_add_codewarrior_configuration(
        TARGETS flash_sdm_hpm_debug flash_sdm_hpm_release
                flash_ldm_hpm_debug flash_ldm_hpm_release
        CC "-D__HPM__"
        CX "-D__HPM__"
    )
endif()

include(${SdkRootDirPath}/examples/_common/project_setting/common.cmake)
