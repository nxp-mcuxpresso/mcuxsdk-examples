:: Copyright 2026 NXP
::
:: SPDX-License-Identifier: BSD-3-Clause

blhost -p COM32 -- flash-erase-region 0x11002000 0x400
blhost -p COM32 write-memory 0x11002000 bootfromflash.bin
blhost -p com32 read-memory 0x11002200 1
pause