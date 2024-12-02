ExternalZephyrProject_Add(
        APPLICATION trdc_secondary_core
        SOURCE_DIR  ${APP_DIR}/../secondary_core
        board ${SB_CONFIG_secondary_board}
        core_id ${SB_CONFIG_secondary_core_id}
        config ${SB_CONFIG_secondary_config}
        toolchain ${SB_CONFIG_secondary_toolchain}
)

# Let's build the secondary application first
add_dependencies(${DEFAULT_IMAGE} trdc_secondary_core)