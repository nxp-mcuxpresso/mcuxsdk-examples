
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/demo_apps/safety_iec60730b/common/cm4_cm7
)


mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/demo_apps/safety_iec60730b/common/cm4_cm7/safety_cm4_cm7_mcx.c
			examples/demo_apps/safety_iec60730b/common/cm4_cm7/safety_cm4_cm7_mcx.h
			examples/demo_apps/safety_iec60730b/common/cm4_cm7/main.c
)

mcux_add_configuration(
    CC "--debug"
    CX "--debug"
)

mcux_remove_iar_configuration(
    AS "--cpu=cortex-m33.no_se"
    CC "--cpu=cortex-m33.no_se"
    CX "--cpu=cortex-m33.no_se"
    LD "--entry Reset_Handler"
)

mcux_add_iar_configuration(
    LD "--keep=__checksum\
       --place_holder __checksum,4,.checksum,4"
    CC "--cmse"
    CX "--cmse"
)

mcux_add_macro(
    CC "-DSKIP_SYSCLK_INIT"
)

# Add or remove Linker File Configurations
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER ${device_root}/MCX/MCXE/MCXE247/arm/MCXE247_flash.scf
)

# Add or remove Linker File Configurations
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    LINKER ${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/linker/mdk/frdmmcxe247_safety_flash.sct
)

# default setting needs to be removed
mcux_remove_iar_configuration(
	TARGETS debug
	CC "-On"
)

mcux_add_iar_configuration(
	CC "-Ol"
	CX "-Ol"
)

# default setting needs to be removed
mcux_remove_mdk_configuration(
    CC "-O1"
    CX "-O1"
)

mcux_add_mdk_configuration(
    CC "-O0"
    CX "-O0"
)

mcux_add_configuration(
    TOOLCHAINS mdk
    CC "-gdwarf-3 -mcmse -g"
	LD "--diag_suppress L6848E --library_type=standardlib"
)

mcux_remove_configuration(
    TOOLCHAINS mdk
	LD "--entry=Reset_Handler"
)

mcux_add_custom_command(
    TOOLCHAINS mdk
    BUILD_EVENT  POST_BUILD
    BUILD_COMMAND ..\\middleware\\safety_iec60730b\\tools\\crc\\crc_hex.bat -..\\..\\..\\..\\build\\mdk\\safety_iec60730b.hex -..\\..\\..\\..\\build\\mdk\\safety_iec60730b_crc.hex -..\\..\\..\\..\\middleware\\safety_iec60730b\\tools\\srecord\\srec_cat.exe
)