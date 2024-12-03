# EdgeLock 2GO Agent (S)

This sample application shows how to use the EdgeLock 2GO service to provisioning keys and certificates to an MCU device. Those keys and certificates can then be used to establish mutual-authenticated TLS connections to cloud services such as AWS or Azure.

Workspace structure:
- *el2go_agent_s*: Project running in the secure processing environment (S)
- *el2go_agent_ns*: Project running in the non-secure processing environment (NS)

Details on building and running the application can be found in the
[el2go_agent_ns](../el2go_agent_ns/readme.md) project.
