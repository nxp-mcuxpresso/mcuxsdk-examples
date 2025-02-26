# MCUboot and ota examples changelog

All notable changes to MCUboot fork in MCUXpresso SDK and related ota examples will be documented in this file.

## [25.03.0] - 03/2025

### Added

- (MCXA20) Basic support for FRDM-MCXA276 and FRDM-MCXA166
- Added SB3 processing support in `ota_mcuboot_basic` example for selected platforms
- (MCXN)(RW61x) Enabled SB3 processing support for FRDM-MCXN947, MCXN9XXEVK, FRDM-RW612 and RD-RW612BGA

### Changed

- Update MCUboot to 2.1.0

### Fixed

- (Encrypted XIP)(IPED) Fixed issues in wifi based examples using IPED encryption
- (Encrypted XIP) Fixed missing default encryption keys

### Removed

- Removed support for MDK toolchain

## [24.12.0] - 12/2024

### Added

- Initial release

### Removed

- Removed WiFi OTA examples on boards that also support Ethernet
