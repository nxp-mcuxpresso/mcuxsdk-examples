# auxiliary

## Overview
The Auxiliary demo application provides a lightweight project skeleton whose main
purpose is to serve an auxiliary role: implementing board-specific functionality
required by a particular board, rather than demonstrating a single peripheral.

Typical use cases include acting as a boot helper (for example kicking off another
core) or serving as an initialization image that prepares external memory so that
another image can be downloaded and debugged afterwards. The exact purpose differs
from board to board.

Because every board has different requirements, the common application code
(`auxiliary.c`) intentionally stays minimal: it only calls `BOARD_InitHardware()`
and then enters an idle loop. The actual auxiliary functionality for each board is
implemented in that board's own `BOARD_InitHardware()` (under the board-specific
example folder), so the behavior of this demo varies from board to board.

## Supported Boards
- [MIMXRT2660-EVK](../../_boards/mimxrt2660evk/demo_apps/auxiliary/example_board_readme.md)
