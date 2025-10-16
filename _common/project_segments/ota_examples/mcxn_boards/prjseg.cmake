# default linker - MCUboot application
if (CONFIG_MCUX_PRJSEG_module.board.ota_examples.mcxn.default_linker_mcuboot)
    mcux_add_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/mcxn10_cm33_flash.icf
    )
    mcux_add_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/mcxn10_cm33_flash.ld
    )
endif()

# custom linker reconfig - MCUboot application
if (CONFIG_MCUX_PRJSEG_module.board.ota_examples.mcxn.custom_linker_mcuboot)
    mcux_add_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/custom_cfg/mcxn10_cm33_flash.icf
    )
    mcux_add_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/ota_examples/mcuboot_opensource/linker/custom_cfg/mcxn10_cm33_flash.ld
    )
endif()

# default linker - OTA application
if (CONFIG_MCUX_PRJSEG_module.board.ota_examples.mcxn.default_linker_application)
    mcux_add_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/ota_examples/ota_linker/mcxn10_cm33_flash.icf
    )
    mcux_add_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/ota_examples/ota_linker/mcxn10_cm33_flash.ld
    )
endif()

# custom linker reconfig - OTA application
if (CONFIG_MCUX_PRJSEG_module.board.ota_examples.mcxn.custom_linker_application)
    mcux_add_iar_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/ota_examples/ota_linker/custom_cfg/mcxn10_cm33_flash.icf
    )
    mcux_add_armgcc_linker_script(
        BASE_PATH ${SdkRootDirPath}
        TARGETS debug release
        LINKER ${board_root}/${board}/ota_examples/ota_linker/custom_cfg/mcxn10_cm33_flash.ld
    )
endif()