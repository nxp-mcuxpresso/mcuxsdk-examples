# Remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT500/MIMXRT595S/iar/MIMXRT595Sxxxx_cm33_ram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT500/MIMXRT595S/arm/MIMXRT595Sxxxx_cm33_ram.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT500/MIMXRT595S/gcc/MIMXRT595Sxxxx_cm33_ram.ld
)

# Add Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/component_examples/mpi_loader/extram_hello_world/iar/MIMXRT595Sxxxx_cm33_psram.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/component_examples/mpi_loader/extram_hello_world/mdk/MIMXRT595Sxxxx_cm33_psram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/component_examples/mpi_loader/extram_hello_world/armgcc/MIMXRT595Sxxxx_cm33_psram.ld
)
