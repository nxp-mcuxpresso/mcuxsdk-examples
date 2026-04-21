mcux_add_include(
  BASE_PATH ${SdkRootDirPath}
  INCLUDES ${board_root}/${board}/eiq_examples/executorch_cifarnet/npu
)
mcux_add_source(
  BASE_PATH ${SdkRootDirPath}
  SOURCES ${board_root}/${board}/eiq_examples/executorch_cifarnet/npu/model_pte.h
)

mcux_remove_armgcc_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_ram.ld
)
mcux_remove_armgcc_linker_script(
  TARGETS flash_debug flash_release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${device_root}/${soc_portfolio}/${soc_series}/${device}/gcc/${CONFIG_MCUX_TOOLCHAIN_LINKER_DEVICE_PREFIX}_flash.ld
)
mcux_add_armgcc_linker_script(
  TARGETS debug release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${board_root}/mimxrt700evk/eiq_examples/executorch_cifarnet/cm33_core0/gcc/MIMXRT798Sxxxx_cm33_core0_ram.ld
)
mcux_add_armgcc_linker_script(
  TARGETS flash_debug flash_release
  BASE_PATH ${SdkRootDirPath}
  LINKER ${board_root}/mimxrt700evk/eiq_examples/executorch_cifarnet/cm33_core0/gcc/MIMXRT798Sxxxx_cm33_core0_flash.ld
)

mcux_add_armgcc_configuration(
  CC "-std=c99"
  CX "-std=c++17"
)
mcux_add_mcux_configuration(
  CC "-std=gnu99"
  CX "-std=gnu++17"
)

mcux_add_armgcc_configuration(
  TARGETS release
  CC "-O3"
  CX "-O3"
)

mcux_add_armgcc_configuration(
    CX "-Wno-sign-compare\
        -Wno-format\
        -Wno-shift-count-overflow\
        -Wno-deprecated-declarations\
        -Wno-int-in-bool-context"
)

mcux_add_mcux_configuration(
    CX "-Wno-sign-compare\
        -Wno-format\
        -Wno-shift-count-overflow\
        -Wno-deprecated-declarations\
        -Wno-int-in-bool-context"
)

mcux_add_macro(
  CC "-DSDK_DEBUGCONSOLE_UART\
      -DARM_MATH_CM33\
      -D__FPU_PRESENT=1\
      -DPRINTF_ADVANCED_ENABLE=1\
      -DPRINTF_FLOAT_ENABLE=1"
  CX "-DSDK_DEBUGCONSOLE_UART\
      -DARM_MATH_CM33\
      -D__FPU_PRESENT=1\
      -DPRINTF_ADVANCED_ENABLE=1\
      -DPRINTF_FLOAT_ENABLE=1"
)

mcux_remove_armgcc_configuration(
  TARGETS release
  CC "-Os"
  CX "-Os"
)

mcux_add_armgcc_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x10000\
      -Xlinker --defsym=__stack_size__=0x10000"
)
mcux_add_mcux_configuration(
  LD "-Xlinker --defsym=__heap_size__=0x10000\
      -Xlinker --defsym=__stack_size__=0x10000"
)

# IMPORTANT: Remove __STARTUP_INITIALIZE_NONCACHEDATA flag for CMSIS-DAP debugger compatibility
# The executorch project uses 1MB ncache region (method_allocator_pool + temp_allocator_pool),
# and zeroing this large region during startup can cause CMSIS-DAP debugger to timeout.
# NonCacheable section variables will be used as-is without initialization.
if(CMAKE_ASM_FLAGS MATCHES "__STARTUP_INITIALIZE_NONCACHEDATA")
    string(REPLACE "-D__STARTUP_INITIALIZE_NONCACHEDATA" "" CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS}")
endif()
if(CMAKE_C_FLAGS MATCHES "__STARTUP_INITIALIZE_NONCACHEDATA")
    string(REPLACE "-D__STARTUP_INITIALIZE_NONCACHEDATA" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
endif()
if(CMAKE_CXX_FLAGS MATCHES "__STARTUP_INITIALIZE_NONCACHEDATA")
    string(REPLACE "-D__STARTUP_INITIALIZE_NONCACHEDATA" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

# Also remove from debug-specific flags
if(CMAKE_C_FLAGS_DEBUG MATCHES "__STARTUP_INITIALIZE_NONCACHEDATA")
    string(REPLACE "-D__STARTUP_INITIALIZE_NONCACHEDATA" "" CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}")
endif()
if(CMAKE_CXX_FLAGS_DEBUG MATCHES "__STARTUP_INITIALIZE_NONCACHEDATA")
    string(REPLACE "-D__STARTUP_INITIALIZE_NONCACHEDATA" "" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
endif()
if(CMAKE_ASM_FLAGS_DEBUG MATCHES "__STARTUP_INITIALIZE_NONCACHEDATA")
    string(REPLACE "-D__STARTUP_INITIALIZE_NONCACHEDATA" "" CMAKE_ASM_FLAGS_DEBUG "${CMAKE_ASM_FLAGS_DEBUG}")
endif()

mcux_add_include(INCLUDES cm33)
