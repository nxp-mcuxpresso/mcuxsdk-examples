
# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXL/MCXL255/iar/MCXL255_cm33_flash.icf
)

mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXL/MCXL255/arm/MCXL255_cm33_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXL/MCXL255/gcc/MCXL255_cm33_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/driver_examples/erm/error_recording_sw/cm33/linkscripts/MCXL255_ermdemo_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/driver_examples/erm/error_recording_sw/cm33/linkscripts/MCXL255_ermdemo_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${board_root}/${board}/driver_examples/erm/error_recording_sw/cm33/linkscripts/MCXL255_ermdemo_flash.ld
)
