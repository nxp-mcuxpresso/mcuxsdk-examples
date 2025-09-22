# ele_concurrent_access

## Overview

This application provides a demonstration of how the SSS API may be used
concurrently from two cores. It demonstrates how initialization may be done
independently on both cores and how to safely prepare input data into
S200-accessible memory if used from the NBU core.

### Access serialization

The S200 enclave supports receiving commands from multiple cores. For such
cases, the S3MU driver functions for sending command messages and receiving
response messages are wrapped in an S3MU-specific semaphore for command
serialization between cores. This serialization is transparent to the driver's
user and the user does not need to manage this semaphore themselves.

However, on top of this there may be a need to serialize access to shared
resources at the application layer, such as access to shared memory.
Such a case is demonstrated by this application, in which the SEMA42 hardware
semaphore is utilized.

### Demo implementation components

The application is divided into three main components:
* the primary core `main.c` file,
* the secondary core `main.c` file,
* and the `common_definitions.h` file.

The individual core's `main.c` implementation files are specific to the given
core. Definitions in the `common_definitions.h` file are the same for both
cores and are included in both core's binaries separately. These definitions
were placed in a single file to reduce code duplication. The common definitions
may differ in small ways between cores; please see the note below or the code
itself for concrete examples.

### NOTE
This example showcases the requirement for buffers that are directly
accessed by the S200 to be placed into S200-accessible memory. Notably,
the S200 may only access Main-core memory and the shared memory between Main and
NBU core. Please note the code sections guarded by the
`#if (defined(IS_RADIO_CORE) && (IS_RADIO_CORE > 0))` construct.

## Building the application

This shows example how to build application for `kw47evk` board with `cm33_core0` core_id.
Change the `-b <board>` parameter based on board you want to build.
Change the `-Dcore_id=<core_id>` parameter based on board core you want to build.
For these parameters please see attribute `boards:` in `primary/example.yml`.

```
west build --sysbuild examples/secure-subsystem_examples/ele_concurrent_access/primary --toolchain armgcc --config debug -b kw47evk -Dcore_id=cm33_core0
```

## Supported Boards
- [KW47-EVK](../../_boards/kw47evk/secure-subsystem_examples/ele_concurrent_access/example_board_readme.md)
