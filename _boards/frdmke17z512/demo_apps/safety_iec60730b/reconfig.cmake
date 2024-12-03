
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/safety_iec60730b/boards/common/cm0/main.c
            middleware/safety_iec60730b/boards/${board}/linker/iar/frdmke17z512_safety.icf
            middleware/safety_iec60730b/boards/${board}/linker/mdk/frdmke17z512_safety.sct
            middleware/safety_iec60730b/boards/${board}/linker/mcux/frdmke17z512_safety.ld
            middleware/safety_iec60730b/boards/${board}/linker/mdk/linker_config.h
            middleware/safety_iec60730b/boards/${board}/safety_test_items.h
            middleware/safety_iec60730b/boards/${board}/safety_test_items.c
            middleware/safety_iec60730b/boards/${board}/isr.h
            middleware/safety_iec60730b/boards/${board}/safety_config.h
            middleware/safety_iec60730b/boards/${board}/device_information.h
            middleware/safety_iec60730b/boards/${board}/project_setup_frdmke17z512.h
            middleware/safety_iec60730b/boards/${board}/project_setup_frdmke17z512.c
            middleware/safety_iec60730b/boards/common/cm0/safety_cm0_kinetis.c
            middleware/safety_iec60730b/boards/common/cm0/safety_cm0_kinetis.h
            middleware/safety_iec60730b/boards/common/cm0/safety.pmp
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
             middleware/safety_iec60730b/boards/frdmke17z512
             middleware/safety_iec60730b/boards/common/cm0
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
)

mcux_add_macro(
    CC "-DFRDM_KE17Z\
       -DFREEDOM"
)

mcux_remove_iar_configuration(
    LD "--entry Reset_Handler"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug
    LINKER devices/Kinetis/KE/MKE17Z9/iar/MKE17Z512xxx9_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug
    LINKER devices/Kinetis/KE/MKE17Z9/arm/MKE17Z512xxx9_flash.scf
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug
    LINKER middleware/safety_iec60730b/boards/${board}/linker/iar/frdmke17z512_safety.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug
    LINKER middleware/safety_iec60730b/boards/${board}/linker/mdk/frdmke17z512_safety.sct
)
