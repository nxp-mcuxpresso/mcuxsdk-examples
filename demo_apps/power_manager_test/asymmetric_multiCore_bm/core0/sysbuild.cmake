ExternalZephyrProject_Add(
        APPLICATION power_manager_test_asymmetric_multiCore_bm_core1
        SOURCE_DIR  ${APP_DIR}/../core1
        board ${SB_CONFIG_secondary_board}
        core_id ${SB_CONFIG_secondary_core_id}
        config ${SB_CONFIG_secondary_config}
        toolchain ${SB_CONFIG_secondary_toolchain}
)

# Let's build the secondary application first
add_dependencies(${DEFAULT_IMAGE} power_manager_test_asymmetric_multiCore_bm_core1)