cmake_minimum_required(VERSION 3.22.0)

include(${SdkRootDirPath}/cmake/extension/mcux.cmake)

project(safety_iec60730b LANGUAGES C CXX ASM PROJECT_BOARD_PORT_PATH ${board_root}/${board}/demo_apps/safety_iec60730b)

# armgcc and MDK post-build command is disabled when 'SAFETY_IEC60730B_ARMGCC_MDK_POSTBUILD' set to 'false'
# srecord tool not available now, so disable post-build command for armgcc and MDK
mcux_set_variable(SAFETY_IEC60730B_ARMGCC_MDK_POSTBUILD false)

# IAR post-build command is disabled when 'SAFETY_IEC60730B_IAR_POSTBUILD' set to 'false', enabled when 'true'
# Disable due to post-build command is not working on Dapeng tests now.
mcux_set_variable(SAFETY_IEC60730B_IAR_POSTBUILD true)

include(${SdkRootDirPath}/CMakeLists.txt)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES
		${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/safety_test_items.h
		${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/safety_test_items.c
		${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/isr.h
		${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/safety_config.h
		${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/device_information.h
		${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/project_setup_${board}.h
		${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/project_setup_${board}.c
		${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/freemaster/freemaster_cfg.h
		${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/pin_mux.c
		${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/pin_mux.h
		${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/readme.md
)

mcux_add_include(
	BASE_PATH ${SdkRootDirPath}
	INCLUDES ${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/linker/mdk
	TOOLCHAINS mdk
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES ${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/linker/mdk/linker_config.h
	TOOLCHAINS mdk
)

mcux_add_include(
	INCLUDES ${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/freemaster
	BASE_PATH ${SdkRootDirPath}
)

mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES ${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}
)

mcux_add_source(
    SOURCES
		common/cm0/startup/cm0/start.c
	CORES cm0p
)

mcux_add_source(
    SOURCES
		common/cm0/startup/cm0/startup_iar.c
		common/cm0/startup/cm0/vectors_iar.c
		common/cm0/startup/cm0/vectors_iar.h
	CORES cm0p
	TOOLCHAINS iar
)

mcux_add_source(
    SOURCES
		common/cm0/startup/cm0/startup_mdk.c
		common/cm0/startup/cm0/vectors_mdk.c
		common/cm0/startup/cm0/vectors_mdk.h
	CORES cm0p
	TOOLCHAINS mdk
)

mcux_add_source(
    SOURCES
		common/cm0/startup/cm0/startup_mcux.c
		common/cm0/startup/cm0/vectors_mcux.c
		common/cm0/startup/cm0/vectors_mcux.h
	CORES cm0p
	TOOLCHAINS mcux armgcc
)

mcux_add_source(
    SOURCES
		common/cm4_cm7/startup/cm4/start.c
	CORES cm4 cm4f
)

mcux_add_source(
    SOURCES
		common/cm4_cm7/startup/cm4/startup_iar.c
		common/cm4_cm7/startup/cm4/vectors_iar.c
		common/cm4_cm7/startup/cm4/vectors_iar.h
	CORES cm4 cm4f
	TOOLCHAINS iar
)

mcux_add_source(
    SOURCES
		common/cm4_cm7/startup/cm4/startup_mdk.c
		common/cm4_cm7/startup/cm4/vectors_mdk.c
		common/cm4_cm7/startup/cm4/vectors_mdk.h
	CORES cm4 cm4f
	TOOLCHAINS mdk
)

mcux_add_source(
    SOURCES
		common/cm4_cm7/startup/cm4/startup_mcux.c
		common/cm4_cm7/startup/cm4/vectors_mcux.c
		common/cm4_cm7/startup/cm4/vectors_mcux.h
	CORES cm4 cm4f
	TOOLCHAINS mcux armgcc
)

mcux_add_source(
    SOURCES
		common/cm4_cm7/startup/cm7/start.c
	CORES cm7 cm7f
)

mcux_add_source(
    SOURCES
		common/cm4_cm7/startup/cm7/startup_iar.c
		common/cm4_cm7/startup/cm7/vectors_iar.c
		common/cm4_cm7/startup/cm7/vectors_iar.h
	CORES cm7 cm7f
	TOOLCHAINS iar
)

mcux_add_source(
    SOURCES
		common/cm4_cm7/startup/cm7/startup_mdk.c
		common/cm4_cm7/startup/cm7/vectors_mdk.c
		common/cm4_cm7/startup/cm7/vectors_mdk.h
	CORES cm7 cm7f
	TOOLCHAINS mdk
)

mcux_add_source(
    SOURCES
		common/cm4_cm7/startup/cm7/startup_mcux.c
		common/cm4_cm7/startup/cm7/vectors_mcux.c
		common/cm4_cm7/startup/cm7/vectors_mcux.h
	CORES cm7 cm7f
	TOOLCHAINS mcux armgcc
)

mcux_add_source(
    SOURCES
		common/cm33/startup/cm33/start.c
	CORES cm33
)

mcux_add_source(
    SOURCES
		common/cm33/startup/cm33/startup_iar.c
		common/cm33/startup/cm33/vectors_iar.c
		common/cm33/startup/cm33/vectors_iar.h
	CORES cm33
	TOOLCHAINS iar
)

mcux_add_source(
    SOURCES
		common/cm33/startup/cm33/startup_mdk.c
		common/cm33/startup/cm33/vectors_mdk.c
		common/cm33/startup/cm33/vectors_mdk.h
	CORES cm33
	TOOLCHAINS mdk
)

mcux_add_source(
    SOURCES
		common/cm33/startup/cm33/startup_mcux.c
		common/cm33/startup/cm33/vectors_mcux.c
		common/cm33/startup/cm33/vectors_mcux.h
	CORES cm33
	TOOLCHAINS mcux armgcc
)

mcux_add_include(
	INCLUDES common/cm0/startup/cm0
	CORES cm0p
)

mcux_add_asm_include(
	INCLUDES common/cm0/startup/cm0
	CORES cm0p
)

mcux_add_include(
	INCLUDES common/cm4_cm7/startup/cm4
	CORES cm4 cm4f
)

mcux_add_asm_include(
	INCLUDES common/cm4_cm7/startup/cm4
	CORES cm4 cm4f
)

mcux_add_include(
	INCLUDES common/cm4_cm7/startup/cm7
	CORES cm7 cm7f
)

mcux_add_asm_include(
	INCLUDES common/cm4_cm7/startup/cm7
	CORES cm7 cm7f
)

mcux_add_include(
	INCLUDES common/cm33/startup/cm33
	CORES cm33
)

mcux_add_asm_include(
	INCLUDES common/cm33/startup/cm33
	CORES cm33
)

mcux_remove_iar_configuration(
    CC "--use_c++_inline"
)


#----------------------------------------------
# Project settings for all toolchains (common for all boards)
#----------------------------------------------

mcux_add_configuration(
    CC "--debug"
    CX "--debug"
)

mcux_add_iar_configuration(
    LD "--keep=__checksum\
       --place_holder __checksum,4,.checksum,4"
)

mcux_add_configuration(
    TOOLCHAINS mdk
	LD "--library_type=standardlib --remove --datacompressor=off"
)

include(${SdkRootDirPath}/${board_root}/${board}/demo_apps/safety_iec60730b/${multicore_foldername}/reconfig.cmake OPTIONAL)

# mcux_convert_binary(BINARY ${APPLICATION_BINARY_DIR}/${MCUX_SDK_PROJECT_NAME}.bin)
