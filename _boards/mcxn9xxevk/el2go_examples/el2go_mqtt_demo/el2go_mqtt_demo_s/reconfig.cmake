mcux_add_macro(
    CC "OCOTP_NV_COUNTERS_RAM_EMULATION=1\
        PSA_WANT_ALG_GCM\
        PSA_WANT_ALG_SHA_1\
        PSA_WANT_KEY_TYPE_RSA_PUBLIC_KEY\
        PSA_WANT_ALG_RSA_PKCS1V15_SIGN"
)

mcux_set_list(
    TFM_LINKER_DEFINES_MQTT "-D__ARM_ARCH_8M_MAIN__ -DCONFIG_TFM_USE_TRUSTZONE -DCONFIG_TFM_PARTITION_META -DENABLE_HEAP"
)

# Optimize for code size
mcux_add_armgcc_configuration(
    CC "-Os"
)

mcux_remove_iar_configuration(
    CC "-On -Oh"
)
mcux_add_iar_configuration(
    CC "-Ohz"
)

mcux_add_mdk_configuration(
    CC "-Oz"
)