
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/safety_iec60730b/boards/common/cm33/main.c
            middleware/safety_iec60730b/boards/${board}/linker/iar/frdmmcxn236_safety_flash.icf
            middleware/safety_iec60730b/boards/${board}/linker/mdk/frdmmcxn236_safety_flash.sct
            middleware/safety_iec60730b/boards/${board}/linker/mdk/linker_config.h
            middleware/safety_iec60730b/boards/${board}/safety_test_items.h
            middleware/safety_iec60730b/boards/${board}/safety_test_items.c
            middleware/safety_iec60730b/boards/${board}/isr.h
            middleware/safety_iec60730b/boards/${board}/safety_config.h
            middleware/safety_iec60730b/boards/${board}/device_information.h
            middleware/safety_iec60730b/boards/${board}/project_setup_frdmmcxn236.h
            middleware/safety_iec60730b/boards/${board}/project_setup_frdmmcxn236.c
            middleware/safety_iec60730b/boards/common/cm33/safety_cm33_mcx.c
            middleware/safety_iec60730b/boards/common/cm33/safety_cm33_mcx.h
            middleware/safety_iec60730b/boards/${board}/freemaster/safety_flash.pmp
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
             middleware/safety_iec60730b/boards/frdmmcxn236
             middleware/safety_iec60730b/boards/common/cm33
             middleware/safety_iec60730b/boards/${board}/core0
             middleware/safety_iec60730b/boards/common/cm0/startup/cm0
             middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm4
             middleware/safety_iec60730b/boards/common/cm4_cm7/startup/cm7
             middleware/safety_iec60730b/boards/common/cm33/startup/cm33
             middleware/safety_iec60730b/boards/${board}/freemaster
)

mcux_add_configuration(
    CC "--debug"
    CX "--debug"
)
mcux_add_iar_configuration(
    LD "--keep=__checksum\
       --place_holder __checksum,4,.checksum,4\
       --entry __iar_program_start"
    CC "--cmse"
    CX "--cmse"
)

mcux_add_macro(
    CC "-DSKIP_SYSCLK_INIT"
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
    LINKER devices/MCX/MCXN/MCXN236/iar/MCXN236_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug
    LINKER devices/MCX/MCXN/MCXN236/arm/MCXN236_flash.scf
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug
    LINKER middleware/safety_iec60730b/boards/${board}/linker/iar/frdmmcxn236_safety_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug
    LINKER middleware/safety_iec60730b/boards/${board}/linker/mdk/frdmmcxn236_safety_flash.sct
)
