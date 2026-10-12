mcux_add_macro(
    TOOLCHAINS armgcc mdk
    TARGETS debug release
    CC "-DCOREMARK_USING_SYSTICK=1\
        -DPRINTF_ADVANCED_ENABLE\
        -DPRINTF_FLOAT_ENABLE\
        -DCOMPILER_VERSION=\"\\\"${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}\\\"\""
)

mcux_remove_armgcc_configuration(
    TARGETS release
    CC "-Os"
)

mcux_remove_mdk_configuration(
    TARGETS release
    CC "-Oz"
)

mcux_add_armgcc_configuration(
    TARGETS release
    CC "-O3 -funroll-all-loops"
)

# Code below to enable LTO for GCC, but Coremark result using LTO is worse.
#mcux_add_armgcc_configuration(
#    TARGETS release
#    CC "-O3 -funroll-all-loops -flto"
#    LD "-flto -u Reset_Handler_C"
#)

mcux_add_mdk_configuration(
    TARGETS release
    CC "-Omax"
    LD "--lto --keep=Reset_Handler_C"
)

mcux_add_armgcc_configuration(
    TARGETS release
    CC "-DCOMPILER_FLAGS=\"\\\"${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}\\\"\""
)

mcux_add_mdk_configuration(
    TARGETS release
    CC "-DCOMPILER_FLAGS=\"\\\"${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}\\\"\""
)