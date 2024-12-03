# mmccard_boot_blocking

## Overview

The MMCCARD Boot project is a demonstration program that uses the SDK
software.The purpose of this example is to show how to use MMCCARD driver
andshow how to use interrupt/polling based transfer API in SDK software driver
to access MMC card boot partition and boot from mmc boot partition.This example
include two sub-examples which is controlled by macro MMC_BOOT_STORE, if the
value is true then the default image and mmc boot configurations will be loaded
to mmc device, the configurations can be modified in the source file, such the
boot bus width and boot timing and so on.If the value is false, re-compile the
project and reset the mmc device power, example will load image from the mmc
device, in this example, image header is loaded first then load the left
image.Note: Make sure store the image and configurations to mmc first.And only
USDHC support this example now.User can use MMC plus card or emmc(on board IC,
but not recommand use emmc socket,due to high speed timing restriction)

## Supported Boards
