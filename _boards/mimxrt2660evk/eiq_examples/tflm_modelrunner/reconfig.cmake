# Copyright 2026 NXP
#
# SPDX-License-Identifier: BSD-3-Clause

# ---------------------------------------------------------------------------
# Dual-mode build control:
#   Phase 1  UART only  -- default: http sources removed, no RTOS/lwIP macros
#   Phase 2  HTTP mode  -- set CONFIG_MODELRUNNER_HTTP=y in your build to
#                          enable HTTP (removes the remove_source block and
#                          adds USE_RTOS=1 / MODELRUNNER_HTTP=1 automatically)
# ---------------------------------------------------------------------------

if(NOT CONFIG_MODELRUNNER_HTTP)
# Phase 1: strip HTTP sources; FreeRTOS and lwIP are not pulled in.
mcux_project_remove_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/tflm_modelrunner/http_handler.cpp
            examples/eiq_examples/tflm_modelrunner/http.cpp
            examples/eiq_examples/tflm_modelrunner/http_handler.h
            examples/eiq_examples/tflm_modelrunner/http.h
)
endif()

# Board-level timer (SysTick -> os_clock_now).
# PSRAM needs no example-side init or access API: it is mapped by the boot
# configuration and used as plain memory (same as tflm_cifar10 on this board).
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/common/timer.c
            examples/eiq_examples/common/timer.h
)

# riscv_elf.S carries the NeutronFirmware ELF embedding for non-IAR toolchains
mcux_add_source(
    BASE_PATH ${SdkRootDirPath}
    SOURCES examples/eiq_examples/common/risc-v/riscv_elf.S
    TOOLCHAINS mdk armgcc mcux
)

# Include paths for timer.h (common) and board-specific headers (app.h, flash_opts.h)
mcux_add_include(
    BASE_PATH ${SdkRootDirPath}
    INCLUDES examples/eiq_examples/common
             ${board_root}/${board}/eiq_examples/tflm_modelrunner
)

# ---------------------------------------------------------------------------
# Compiler macros
# ---------------------------------------------------------------------------
mcux_add_macro(
    CC "-DSDK_DEBUGCONSOLE_UART\
       -DARM_MATH_CM85\
       -DUSE_NPU=1\
       -DMODEL_SIZE=500*1024\
       -DTENSORARENA_DATA=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -D__FPU_PRESENT=1"
    CX "-DSDK_DEBUGCONSOLE_UART\
       -DARM_MATH_CM85\
       -DUSE_NPU=1\
       -DMODEL_SIZE=500*1024\
       -DTENSORARENA_DATA=1\
       -DPRINTF_ADVANCED_ENABLE=1\
       -DPRINTF_FLOAT_ENABLE=1\
       -D__FPU_PRESENT=1"
)

# LWIP_TIMEVAL_PRIVATE=0 is harmless in UART mode; avoids re-build for HTTP
mcux_add_macro(
    TOOLCHAINS armgcc
    CC "-DLWIP_TIMEVAL_PRIVATE=0"
    CX "-DLWIP_TIMEVAL_PRIVATE=0"
)

# Phase 2 HTTP mode: add FreeRTOS + lwIP + ENET macros and pull in the
# board-level ENET hardware_init / app.h from the lwip common layer.
if(CONFIG_MODELRUNNER_HTTP)
mcux_add_macro(
    CC "-DUSE_RTOS=1\
       -DMODELRUNNER_HTTP=1\
       -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
       -DBOARD_USE_PCAL6524=1\
       -DSDK_I2C_BASED_COMPONENT_USED=1"
    CX "-DUSE_RTOS=1\
       -DMODELRUNNER_HTTP=1\
       -DFSL_SDK_ENABLE_DRIVER_CACHE_CONTROL=1\
       -DBOARD_USE_PCAL6524=1\
       -DSDK_I2C_BASED_COMPONENT_USED=1"
)
# ENET netif init function expected by http.cpp stack_init()
mcux_add_macro(
    CC "-DEXAMPLE_NETIF_INIT_FN=ethernetif0_init"
    CX "-DEXAMPLE_NETIF_INIT_FN=ethernetif0_init"
)
endif()

# ---------------------------------------------------------------------------
# IAR configuration
# ---------------------------------------------------------------------------
mcux_remove_iar_configuration(
    CC "-Oh -On"
)
mcux_add_iar_configuration(
    CC "--dlib_config full\
       -Ohs\
       --diag_suppress=Pe068,Pa025\
       --diag_suppress=Pe260,Pe1031"
    LD "--redirect _Printf=_PrintfFull"
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

# ---------------------------------------------------------------------------
# MDK (armclang) configuration
# ---------------------------------------------------------------------------
mcux_add_mdk_configuration(
    CX "-std=gnu++17"
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
mcux_remove_mdk_configuration(
    CC "-fshort-wchar"
    CX "-fshort-wchar"
)
mcux_remove_mdk_configuration(
    TARGETS release
    CC "-O3\
       -Oz"
)

# ---------------------------------------------------------------------------
# armgcc configuration
# ---------------------------------------------------------------------------
mcux_remove_armgcc_configuration(
    LD "--specs=nano.specs"
)
mcux_remove_armgcc_configuration(
    TARGETS xspi_nor_psram_release
    CC "-Os"
    CX "-Os"
)

# ---------------------------------------------------------------------------
# Linker script replacement for xspi_nor_psram targets
# The modelrunner-specific scripts enlarge the PSRAM non-cacheable region to
# hold the tensor arena and the model-data heap.
# ---------------------------------------------------------------------------
mcux_remove_armgcc_linker_script(
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/RT/RT2660/MIMXRT2663/gcc/MIMXRT2663xxxxx_xspi_nor_psram.ld
)
mcux_add_armgcc_linker_script(
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/mimxrt2660evk/eiq_examples/tflm_modelrunner/gcc/MIMXRT2663xxxxx_xspi_nor_psram.ld
)

mcux_remove_iar_linker_script(
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER devices/RT/RT2660/MIMXRT2663/iar/MIMXRT2663xxxxx_xspi_nor_psram.icf
)
mcux_add_iar_linker_script(
    TARGETS xspi_nor_psram_debug xspi_nor_psram_release
    BASE_PATH ${SdkRootDirPath}
    LINKER examples/_boards/mimxrt2660evk/eiq_examples/tflm_modelrunner/iar/MIMXRT2663xxxxx_xspi_nor_psram.icf
)

# ---------------------------------------------------------------------------
# Heap / stack sizing -- tensor arena lives in PSRAM non-cacheable region
# ---------------------------------------------------------------------------
mcux_add_armgcc_configuration(
    LD "-Xlinker --defsym=__heap_size__=0x320000\
        -Xlinker --defsym=__heap_noncacheable__=1\
        -Xlinker --defsym=__stack_size__=0x9000"
)
mcux_add_iar_configuration(
    LD "--config_def=__stack_size__=0x16000\
        --config_def=__heap_size__=0x150000\
        --config_def=__heap_noncacheable__=1"
)

# ---------------------------------------------------------------------------
# NeutronFirmware embedding (IAR only -- armgcc/MDK use riscv_elf.S above)
# ---------------------------------------------------------------------------
mcux_add_iar_configuration(
    LD "--image_input=${SdkRootDirPath}/middleware/eiq_int/neutron/rt2660/NeutronFirmware.elf,_binary_NeutronFirmware_elf_start,.elfdata,16\
        --keep=_binary_NeutronFirmware_elf_start"
)
