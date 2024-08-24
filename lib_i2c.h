/******************************************************************************
* Lightweight and simple CH32V0003 I2C Library.
*
* This library provides functions to init, read and write to the hardware I2C
* Bus - in Default, and Alternative Pinout Modes.
* Default:	SCL = PC2		SDA = PC1
* Alt 1:	SCL = PD1		SDA = PD0
* Alt 2:	SCL = PC5		SDA = PC6
*
* See GitHub Repo for more information: 
* https://github.com/ADBeta/CH32V000x-lib_i2c
*
* 20 Aug 2024    Version 2.4
*
* Released under the MIT Licence
* Copyright ADBeta (c) 2024
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the 
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
* sell copies of the Software, and to permit persons to whom the Software is 
* furnished to do so, subject to the following conditions:
* The above copyright notice and this permission notice shall be included in 
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
* EXPRESS OR IMPLIED, INCLUDING BUT N00: default mapping (SCL/PC2, SDA/PC1).
01: Remapping (SCL/ PD1, SDA/ PD0).
1X: Remapping (SCL/PC5, SDA/PC6)OT LIMITED TO THE WARRANTIES OF 
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
* DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR 
* OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE 
* USE OR OTHER DEALINGS IN THE SOFTWARE.
******************************************************************************/
#ifndef CH32_LIB_I2C_H
#define CH32_LIB_I2C_H

// TODO: Assess why the I2C Bus halts and doesn't recover from pins being 
// swapped (look at the registers in real-time)


#include "ch32v003fun.h"

// TESTED: DEFAULT OK    ALT_1 OK    
#define I2C_PINOUT_DEFAULT
//#define I2C_PINOUT_ALT_1
//#define I2C_PINOUT_ALT_2

/*** Hardware Definitions ****************************************************/
// Common Clock Speeds
#define I2C_CLK_400KHZ 400000
#define I2C_CLK_100KHZ 100000

// Hardware CLK Prerate and timeout
#define I2C_PRERATE 2000000  // NOTE: test is actually correct?
#define I2C_TIMEOUT 2000

// Default Pinout
#ifdef I2C_PINOUT_DEFAULT
	#define I2C_AFIO_REG    ((uint32_t)0x00000000)
	#define I2C_PORT_RCC	RCC_APB2Periph_GPIOC
	#define I2C_PORT		GPIOC
	#define I2C_PIN_SCL 	2
	#define I2C_PIN_SDA 	1
#endif

// Alternate 1 Pinout
#ifdef I2C_PINOUT_ALT_1
	#define I2C_AFIO_REG    ((uint32_t)0x04000002)
	#define I2C_PORT_RCC	RCC_APB2Periph_GPIOD
	#define I2C_PORT		GPIOD
	#define I2C_PIN_SCL 	1
	#define I2C_PIN_SDA 	0
#endif

// Alternate 2 Pinout
#ifdef I2C_PINOUT_ALT_2
	#define I2C_AFIO_REG    ((uint32_t)0x00400002)
	#define I2C_PORT_RCC	RCC_APB2Periph_GPIOC
	#define I2C_PORT		GPIOC
	#define I2C_PIN_SCL 	5
	#define I2C_PIN_SDA 	6
#endif

// Error Code Definitons
typedef enum {
	I2C_OK      = 0,  // No Error. All OK
	I2C_ERR_TIMEOUT,  // Timeout happened when read/write
	I2C_ERR_NACK,     // ACK Bit failed
	I2C_ERR_BERR,     // Bus Error
	I2C_ERR_ARLO,     // Arbitration Lost
} i2c_err_t;

/*** Static Functions ********************************************************/
/// @breif Checks the I2C Status against a mask value, returns 1 if it matches
/// @param Status To match to
/// @return uint32_t masked status value: 1 if mask and status match
__attribute__((always_inline))
static inline uint32_t i2c_status(const uint32_t status_mask)
{
	uint32_t status = (uint32_t)I2C1->STAR1 | (uint32_t)(I2C1->STAR2 << 16);
	return (status & status_mask) == status_mask; 
}


/*** Functions ***************************************************************/
/// @breif Initialise the I2C Peripheral on the default pins, in Master Mode
/// @param clk_rate that the I2C Bus should use in Hz. Max 400000
/// @return i2c_err_t - I2C_OK On success
i2c_err_t i2c_init(const uint32_t clk_rate);

/// @breif Pings a specific I2C Address, and returns a i2c_err_t status
/// @param addr I2C Device Address, MUST BE 7 Bit
/// @return i2c_err_t, I2C_OK if the device responds, 
/// I2C_ERR_TIMOUT or I2C_ERR_NO_ACK if not
i2c_err_t i2c_ping(const uint8_t addr);

/// @breif Scans through all 7 Bit addresses, prints any that respond
/// @param None
/// @return None
void i2c_scan(void);

/// @breif reads [len] bytes from [addr]s [reg] register into [buf]
/// @param addr, address of I2C Device to Read from, MUST BE 7 Bit
/// @param buf, buffer to read to
/// @param len, number of bytes to read
/// @return 12c_err_t. I2C_OK on Success, TIMEOUT or NO_DATA on error
i2c_err_t i2c_read(const uint8_t addr,    const uint8_t reg,
				                          uint8_t *buf,
                                          const uint8_t len);

/// @breif writes [len] bytes from [buf], to the [reg] of [addr]
/// @param addr, Address of the I2C Device to Write to, MUST BE 7 Bit
/// @param buf, Buffer to write from
/// @param len, number of bytes to read
/// @return i2c_err_t. I2C_OK On Success. TIMEOUT or TODO:
i2c_err_t i2c_write(const uint8_t addr,    const uint8_t reg,
					                       const uint8_t *buf,
                                           const uint8_t len);

#endif
