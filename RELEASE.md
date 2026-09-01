# Emulated EEPROM Library Release Notes

### What's Included?
Please refer to the [README.md](./README.md) and the [API Reference Guide](https://infineon.github.io/emeeprom/html/index.html) for a complete description of the Emulated EEPROM Middleware.
The revision history of the Emulated EEPROM Middleware is also available on the [API Reference Guide Changelog](https://github.com/Infineon/emeeprom/blob/master/RELEASE.md).

### What Changed?
Release version v2.80.0:
* Added at-init corrupt-sector recovery

Release version v2.70.1:
* Documentation update

Release version v2.70.0:
* Documentation update

Release version v2.60.0:
* Updated implementation based on program size

Release version v2.50.0:
* Added support for MXS40v2 devices

Release version v2.40.0:
* Moved RAM buffer to file scope to avoid overflowing the stack

Release version v2.30.1:
* Bug fixes and BWC fixes
* Updated documentation
* Updated patch version

Release version v2.30:
* Introduced block storage implementation

Release version v2.20:
* Added support for XMC7xxx devices
* Added support for T2G-B-H devices
* Updated documentation
* Updated minor version

Release version v2.10:
* Added support for PSoC 4 devices
* Updated documentation
* Updated minor version

Release version v2.0:
* Updated major and minor version defines for consistency with other libraries
* Updated documentation for user experience improvement
* Added migration guide from PSoC Creator Em EEPROM component
* Added mechanism to restore corrupted redundant copy from the main data copy

### Defect Fixes
* Fixed: on supported eCT-flash devices (XMC7xxx, XMC5xxx), a power failure during sector erase could leave the sector in a partially-erased state with NC-ECC errors, causing bus faults on the next CPU read. The middleware now detects and erases such sectors at init time.


---
(c) 2019-2026, Infineon Technologies AG, or an affiliate of Infineon
Technologies AG.  All rights reserved.
