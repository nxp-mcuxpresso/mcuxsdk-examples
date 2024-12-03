
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_macro(
    CC "-DSDK_I2C_BASED_COMPONENT_USED=1\
       -DBOARD_USE_PCA6416A=1"
)

mcux_remove_macro(
    AS "-D__STARTUP_INITIALIZE_NONCACHEDATA"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX8ULP/MIMX8UD7/iar/MIMX8UD7xxxxx_ram.icf
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/i.MX/i.MX8ULP/MIMX8UD7/iar/MIMX8UD7xxxxx_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX8ULP/MIMX8UD7/gcc/MIMX8UD7xxxxx_ram.ld
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/i.MX/i.MX8ULP/MIMX8UD7/gcc/MIMX8UD7xxxxx_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/iar/MIMX8UD7xxxxx_cm33_place_noncacaheable_section_to_dram_ram.icf
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/iar/MIMX8UD7xxxxx_cm33_place_noncacaheable_section_to_dram_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/gcc/MIMX8UD7xxxxx_cm33_place_noncacaheable_section_to_dram_ram.ld
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/gcc/MIMX8UD7xxxxx_cm33_place_noncacaheable_section_to_dram_flash.ld
)
