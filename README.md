# CH32V003_lib_i2c
**This is a simplified library for I2C, used with [ch32v003_fun](https://github.com/cnlohr/ch32v003fun)**

## Library Description
This Library provides a hardware peripheral I2C interface, with the following features:
* Support for Alternative pinouts (WIP)
* Support 7-bit Addresses (7-bit aligned, eg `0bx1101000 - 0x68`)
* Support 8-bit Registers
* Easy to use I2C Error Status'
* Funcion to Scan the Interface for devices
* Master Mode Only

## TODO
* Test on other MCU Variants:
	* CH32V003 ✔️


If you want to use ANY pins for I2C, see [lib_swi2c](https://github.com/ADBeta/CH32Vxxx_lib_swi2c)

----
MIT License
Copyright (c) 2024 ADBeta
