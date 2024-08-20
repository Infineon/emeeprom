# Emulated EEPROM Library

## Overview
The Emulated EEPROM middleware emulates an EEPROM device in the device non volatile-memory.
The EEPROM middleware operates on the top of the block storage library that abstracts memories by specifying
how to perform memory operations such as read, write, erase, and providing auxiliary functions to retrieve erase and program size and checking whether an address is whithin the memory range.

Use the Emulated EEPROM to store nonvolatile data on a target device, with wear leveling and restoring corrupted data from a redundant copy.

## Features
* EEPROM-Like Non-Volatile Storage
* Easy to use Read and Write API
* Optional Wear Leveling
* Optional Redundant Data storage
* Supports any storage which can be modeled as a block device, including internal nvm or external flash
(e.g. via QSPI).
* Designed to be resilient to power failures.
* Designed to promote even wear of the storage (when wear leveling is enabled).

## Dependencies
* block-storage

## Quick Start
Refer to the API Reference Quick Start section for step-by-step instruction how to enable the Emulated EEPROM Middleware Library.

## More information
For more information, refer to the following documents:
* [Emulated EEPROM Middleware API Reference Guide](https://infineon.github.io/emeeprom/html/index.html)
* [Cypress Semiconductor, an Infineon Technologies Company](http://www.cypress.com)
* [Infineon GitHub](https://github.com/infineon)
* [ModusToolbox�](https://www.cypress.com/products/modustoolbox-software-environment)

---
� Cypress Semiconductor Corporation (an Infineon company) or an affiliate of Cypress Semiconductor Corporation, 2023.
