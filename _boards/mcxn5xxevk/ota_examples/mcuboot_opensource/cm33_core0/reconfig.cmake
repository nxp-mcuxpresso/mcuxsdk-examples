# Add additional configuration
mcux_add_macro(
    CC "-DENCRYPTED_XIP_NPX"
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/ota_examples/mcuboot_opensource
)

mcux_add_mdk_configuration(
    LD "--diag_suppress=L6329W"
)

mcux_add_armgcc_configuration(
    TARGETS debug
    CC "-Og"
)

mcux_add_iar_configuration(
    TARGETS release
    CC "-Ohz"
)

# Remove additional configuration
mcux_remove_armgcc_configuration(
    TARGETS debug
    CC "-O0"
)

mcux_remove_iar_configuration(
    TARGETS release
    CC "-Oh"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXN/MCXN547/iar/MCXN547_cm33_core0_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXN/MCXN547/gcc/MCXN547_cm33_core0_flash.ld
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER ${device_root}/MCX/MCXN/MCXN547/arm/MCXN547_cm33_core0_flash.scf
)

# Specific linkers are added using project segments
# see examples/_common/project_segments/ota_examples/mcxn_boards/prjseg.cmake
