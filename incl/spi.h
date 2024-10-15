/* spi.h
 *
 * header file for spi.c
 */

#ifndef _SPI_H
#define _SPI_H

#include <mcp.h>
#include <gpio.h>

#define	RCGCSSI 	0x400FE61C
#define SSICR0  	0x000    	// SSI Control 0
#define SSICR1  	0x004    	// SSI Control 0
#define SSIDATA 	0x008     	// SSI Data Register
#define SSISR   	0x00C     	// SSI Register Status
#define	SSICPSR		0x010    	// Clock Prescale reg
#define	SSICC		0xFC8    	// clock source

#define SSI0_BASE	0x40008000
#define SSI1_BASE	0x40009000
#define SSI2_BASE	0x4000A000
#define SSI3_BASE	0x4000B000

typedef struct tiva_spi {
	volatile void *ssi_base;
	volatile void *gpio_base;
} tiva_spi;


tiva_spi *spi_init(uint8_t ssi_num);
void spi_txwait(tiva_spi *spi);

#endif
