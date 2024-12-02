
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES middleware/safety_iec60730b/boards/${board}/main.c
            middleware/safety_iec60730b/boards/${board}/linker/iar/frdmrw612_safety_flash.icf
            middleware/safety_iec60730b/boards/${board}/isr.h
            middleware/safety_iec60730b/boards/${board}/safety_config.h
            middleware/safety_iec60730b/boards/${board}/device_information.h
            middleware/safety_iec60730b/boards/${board}/project_setup_frdmrw612.h
            middleware/safety_iec60730b/boards/${board}/project_setup_frdmrw612.c
            middleware/safety_iec60730b/boards/common/cm33/safety_cm33_RW61x.c
            middleware/safety_iec60730b/boards/common/cm33/safety_cm33_RW61x.h
            middleware/safety_iec60730b/boards/${board}/freemaster/safety_flash.pmp
            middleware/safety_iec60730b/boards/${board}/pin_mux.c
            middleware/safety_iec60730b/boards/${board}/pin_mux.h
            middleware/safety_iec60730b/boards/${board}/startup/start.c
            middleware/safety_iec60730b/boards/${board}/startup/startup_iar.c
            middleware/safety_iec60730b/boards/${board}/startup/vectors_iar.c
            middleware/safety_iec60730b/boards/${board}/startup/vectors_iar.h
            middleware/safety_iec60730b/boards/${board}/freemaster/freemaster_cfg.h
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES middleware/safety_iec60730b/boards/frdmrw612
             middleware/safety_iec60730b/boards/common/cm33
             middleware/safety_iec60730b/boards/${board}/startup
             middleware/safety_iec60730b/boards/${board}/freemaster
)

mcux_add_configuration(
    CC "--debug"
    CX "--debug"
)
mcux_add_iar_configuration(
    LD "--keep=__checksum\
       --place_holder __checksum,4,.checksum,4\
       --entry __iar_program_start\
       --diag_suppress Lt069"
    AS "-S"
    CC "--cmse\
       --diag_suppress Pa082,Pa050,Pe188,Pe069"
    CX "--cmse\
       -e\
       --diag_suppress Pa082,Pa050,Pe188,Pe069"
)

mcux_add_macro(
    CC "-DSKIP_SYSCLK_INIT"
)

mcux_remove_iar_configuration(
    LD "--entry Reset_Handler"
)

mcux_remove_macro(
    CC "-DMCUXPRESSO_SDK"
)

# Add or remove Linker File Configurations
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug
    LINKER devices/Wireless/RW/RW612/iar/RW612_ram.icf
)

# Add or remove Linker File Configurations
mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug
    LINKER middleware/safety_iec60730b/boards/${board}/linker/iar/frdmrw612_safety_flash.icf
)
