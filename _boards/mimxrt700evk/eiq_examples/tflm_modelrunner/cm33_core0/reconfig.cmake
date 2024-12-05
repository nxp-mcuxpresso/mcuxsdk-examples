mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/tflm_modelrunner/http_handler.cpp
            examples/eiq_examples/tflm_modelrunner/http.cpp
            examples/eiq_examples/tflm_modelrunner/http_handler.h
            examples/eiq_examples/tflm_modelrunner/http.h
)

mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/common/timer.c
            examples/eiq_examples/common/timer.h
            examples/_boards/${board}/eiq_examples/tflm_modelrunner/${core_id}/app.h
	    examples/driver_examples/xspi/psram/polling_transfer/xspi_psram_ops.c
)

mcux_remove_iar_configuration(
	CC "-Oh -On"
)


mcux_add_mdk_configuration(
	CX "-std=gnu++17"
)
mcux_add_iar_configuration(
    CC "--dlib_config full\
       -Ohs\
       --diag_suppress=Pe068,Pa025\
       --diag_suppress=Pe260,Pe1031"
    LD "--redirect _Printf=_PrintfFull"
)
mcux_add_mdk_configuration(
    LD "--predefine=-D__stack_size__=0x8000\
       --predefine=-D__heap_size__=0x150000\
       --library_type=nomicrolib"
)
mcux_add_mdk_configuration(
    TARGETS release
    CC "-Oz\
       -O3"
)
mcux_add_armgcc_configuration(
    TARGETS release
    CC "-O3"
    CX "-O3"
)

mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
       -DARM_MATH_CM33\
       -DUSE_NPU=1\
       -DEXTERNAL_MEM=1\
       -DMODEL_SIZE=1000*1024\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -D__FPU_PRESENT=1"
    CX "-DSDK_DEBUGCONSOLE_UART\
       -DARM_MATH_CM33\
       -DUSE_NPU=1\
       -DEXTERNAL_MEM=1\
       -DMODEL_SIZE=1000*1024\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -D__FPU_PRESENT=1"
)
mcux_add_macro(
    TOOLCHAINS armgcc
    CC "-DLWIP_TIMEVAL_PRIVATE=0"
    CX "-DLWIP_TIMEVAL_PRIVATE=0"
)

mcux_remove_iar_configuration(
    TARGETS debug
    CC "--no_scheduling"
    CX "--no_scheduling"
)
mcux_remove_iar_configuration(
    TARGETS release
    CC "-Om"
)
mcux_remove_mdk_configuration(
    CC "-fshort-wchar"
    CX "-fshort-wchar"
)
mcux_remove_mdk_configuration(
    TARGETS release
    CC "-O3\
       -Oz"
)
mcux_remove_armgcc_configuration(
    LD "--specs=nano.specs"
)
mcux_remove_armgcc_configuration(
    TARGETS release
    CC "-Os"
    CX "-Os"
)

mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT700/MIMXRT798S/iar/MIMXRT798Sxxxx_cm33_core0_flash.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT700/MIMXRT798S/arm/MIMXRT798Sxxxx_cm33_core0_flash.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER devices/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld
)
mcux_remove_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/iar/MIMXRT798Sxxxx_cm33_core0_ram.icf
)
mcux_remove_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/arm/MIMXRT798Sxxxx_cm33_core0_ram.scf
)
mcux_remove_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER devices/RT/RT700/MIMXRT798S/gcc/MIMXRT798Sxxxx_cm33_core0_ram.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/eiq_examples/tflm_modelrunner/cm33_core0/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/eiq_examples/tflm_modelrunner/cm33_core0/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS flash_debug flash_release
    LINKER examples/_boards/${board}/eiq_examples/tflm_modelrunner/cm33_core0/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)

mcux_add_iar_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/eiq_examples/tflm_modelrunner/cm33_core0/iar/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.icf
)
mcux_add_mdk_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/eiq_examples/tflm_modelrunner/cm33_core0/arm/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.scf
)
mcux_add_armgcc_linker_script(
    BASE_PATH ${SdkRootDirPath}
    TARGETS debug release
    LINKER examples/_boards/${board}/eiq_examples/tflm_modelrunner/cm33_core0/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
)

mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x16000\
        --config_def=__heap_size__=0x150000"
)
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__heap_size__=0x150000\
        -Xlinker --defsym=__stack_size__=0x16000"
)
