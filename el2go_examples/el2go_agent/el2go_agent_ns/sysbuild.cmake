ExternalMCUXProject_Add(
    APPLICATION el2go_agent_s
    SOURCE_DIR  ${APP_DIR}/../el2go_agent_s
)

add_dependencies(${DEFAULT_IMAGE} el2go_agent_s)