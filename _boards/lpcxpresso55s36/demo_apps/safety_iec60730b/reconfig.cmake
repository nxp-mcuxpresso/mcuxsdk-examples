
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/safety_iec60730b/boards/common/cm33/main.c
            middleware/safety_iec60730b/boards/${board}/linker/iar/lpcxpresso55s36_safety.icf
            middleware/safety_iec60730b/boards/${board}/linker/mdk/lpcxpresso55s36_safety.sct
            middleware/safety_iec60730b/boards/${board}/linker/mcux/lpcxpresso55s36_safety.ld
            middleware/safety_iec60730b/boards/${board}/linker/mdk/linker_config.h
            middleware/safety_iec60730b/boards/${board}/safety_test_items.h
            middleware/safety_iec60730b/boards/${board}/safety_test_items.c
            middleware/safety_iec60730b/boards/${board}/isr.h
            middleware/safety_iec60730b/boards/${board}/safety_config.h
            middleware/safety_iec60730b/boards/${board}/device_information.h
            middleware/safety_iec60730b/boards/${board}/project_setup_lpcxpresso55s36.h
            middleware/safety_iec60730b/boards/${board}/project_setup_lpcxpresso55s36.c
            middleware/safety_iec60730b/boards/common/cm33/safety_cm33_lpc.c
            middleware/safety_iec60730b/boards/common/cm33/safety_cm33_lpc.h
            middleware/safety_iec60730b/boards/common/cm33/safety.pmp
            middleware/safety_iec60730b/boards/${board}/clock_config.c
            middleware/safety_iec60730b/boards/${board}/clock_config.h
            middleware/safety_iec60730b/boards/${board}/freemaster/freemaster_cfg.h
            middleware/safety_iec60730b/boards/${board}/pin_mux.c
            middleware/safety_iec60730b/boards/${board}/pin_mux.h
            middleware/safety_iec60730b/tools/crc/crc_hex.bat
            middleware/safety_iec60730b/boards/common/cm0/startup/cm0/start.c
            middleware/safety_iec60730b/boards/common/cm0/startup/cm0/startup_iar.c
            middleware/safety_iec60730b/boards/common/cm0/startup/cm0/vectors_iar.c
            middleware/safety_iec60730b/boards/common/cm0/startup/cm0/vectors_iar.h
            middleware/safety_iec60730b/boards/common/cm0/startup/cm0/startup_mdk.c
            middleware/safety_iec60730b/boards/common/cm0/startup/cm0/vectors_mdk.c
            middleware/safety_iec60730b/boards/common/cm0/startup/cm0/vectors_mdk.h
            middleware/safety_iec60730b/boards/common/cm0/startup/cm0/startup_mcux.c
            middleware/safety_iec60730b/boards/common/cm0/startup/cm0/vectors_mcux.c
            middleware/safety_iec60730b/boards/common/cm0/startup/cm0/vectors_mcux.h
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm4/start.c
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm4/startup_iar.c
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm4/vectors_iar.c
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm4/vectors_iar.h
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm4/startup_mdk.c
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm4/vectors_mdk.c
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm4/vectors_mdk.h
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm4/startup_mcux.c
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm4/vectors_mcux.c
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm4/vectors_mcux.h
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm7/start.c
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm7/startup_iar.c
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm7/vectors_iar.h
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm7/vectors_iar.c
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm7/startup_mdk.c
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm7/vectors_mdk.c
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm7/vectors_mdk.h
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm7/startup_mcux.c
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm7/vectors_mcux.c
            middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm7/vectors_mcux.h
            middleware/safety_iec60730b/boards/common/cm33/startup/cm33/start.c
            middleware/safety_iec60730b/boards/common/cm33/startup/cm33/startup_iar.c
            middleware/safety_iec60730b/boards/common/cm33/startup/cm33/vectors_iar.c
            middleware/safety_iec60730b/boards/common/cm33/startup/cm33/vectors_iar.h
            middleware/safety_iec60730b/boards/common/cm33/startup/cm33/startup_mdk.c
            middleware/safety_iec60730b/boards/common/cm33/startup/cm33/vectors_mdk.c
            middleware/safety_iec60730b/boards/common/cm33/startup/cm33/vectors_mdk.h
            middleware/safety_iec60730b/boards/common/cm33/startup/cm33/startup_mcux.c
            middleware/safety_iec60730b/boards/common/cm33/startup/cm33/vectors_mcux.c
            middleware/safety_iec60730b/boards/common/cm33/startup/cm33/vectors_mcux.h
            middleware/safety_iec60730b/boards/common/safety_debug.ini
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/safety_iec60730b/boards/${board}/linker/mdk
             middleware/safety_iec60730b/boards/lpcxpresso55s36
             middleware/safety_iec60730b/boards/common/cm33
             middleware/safety_iec60730b/boards/common/cm0/startup/cm0
             middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm4
             middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm7
             middleware/safety_iec60730b/boards/common/cm33/startup/cm33
             middleware/safety_iec60730b/boards/${board}/freemaster
)

mcux_add_iar_configuration(
    LD "--keep=__checksum\
       --place_holder __checksum,4,.checksum,4\
       --entry __iar_program_start"
    CC "--cmse"
    CX "--cmse"
)

mcux_add_macro(
    TOOLCHAINS mdk
    CC "-D_DEBUG=1"
)

mcux_remove_iar_configuration(
    AS "--cpu=cortex-m33.no_se"
    CC "--cpu=cortex-m33.no_se"
    CX "--cpu=cortex-m33.no_se"
    LD "--entry Reset_Handler"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug
    LINKER devices/LPC/LPC5500/LPC55S36/iar/LPC55S36_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug
    LINKER devices/LPC/LPC5500/LPC55S36/arm/LPC55S36_flash.scf
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug
    LINKER middleware/safety_iec60730b/boards/${board}/linker/iar/lpcxpresso55s36_safety.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug
    LINKER middleware/safety_iec60730b/boards/${board}/linker/mdk/lpcxpresso55s36_safety.sct
)
