ExternalMCUXProject_Add(
        APPLICATION freertos_mpu_s
        SOURCE_DIR  ${APP_DIR}/../freertos_mpu_s
        board ${board}
        core_id ${core_id}
        config ${config}
        toolchain ${toolchain}
)

# Let's build the secure application first
add_dependencies(${DEFAULT_IMAGE} freertos_mpu_s)
