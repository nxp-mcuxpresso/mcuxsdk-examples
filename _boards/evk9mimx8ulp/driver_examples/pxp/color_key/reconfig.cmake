mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/${board}/display_support.h
            examples/_boards/${board}/display_support.c
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/_boards/evkmimx8ulp/display_support.h
            examples/_boards/evkmimx8ulp/display_support.c
            examples/_boards/evkmimx8ulp/driver_examples/pxp/color_key/./hardware_init.c
            examples/_boards/evkmimx8ulp/driver_examples/pxp/color_key/./app.h
            middleware/multicore/remoteproc/empty_rsc_table.c
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/_boards/evkmimx8ulp/driver_examples/pxp/color_key
             examples/_boards/evkmimx8ulp
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
    LINKER devices/i.MX/i.MX8ULP/MIMX8UD5/iar/MIMX8UD5xxxxx_ram.icf
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/i.MX/i.MX8ULP/MIMX8UD5/iar/MIMX8UD5xxxxx_flash.icf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/i.MX/i.MX8ULP/MIMX8UD5/gcc/MIMX8UD5xxxxx_ram.ld
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/i.MX/i.MX8ULP/MIMX8UD5/gcc/MIMX8UD5xxxxx_flash.ld
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/evkmimx8ulp/iar/MIMX8UD7xxxxx_cm33_place_noncacaheable_section_to_dram_ram.icf
)
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/evkmimx8ulp/iar/MIMX8UD7xxxxx_cm33_place_noncacaheable_section_to_dram_flash.icf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/evkmimx8ulp/gcc/MIMX8UD7xxxxx_cm33_place_noncacaheable_section_to_dram_ram.ld
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/evkmimx8ulp/gcc/MIMX8UD7xxxxx_cm33_place_noncacaheable_section_to_dram_flash.ld
)
