/* ===========================================================================*\
 * spi.c
 *
 * spi functions
 *
 * INCOMPLETE (only implements what I've needed so far)
 *
 *
 * CONTENTS:
 *============================================================================*/

#include <spi.h>

/* -- spi_init -----------------------------------------------------------------
 * current limitations: defaults to sysclk, prescaler=4, & 8bit data mode
 * also, master and polarity */
tiva_spi *spi_init(uint8_t ssi_num)
{
	// check ssi_num valid
	if (ssi_num < 0 || 3 < ssi_num){
		return -1;
	}

	/* 'malloc' tiva_spi struct */
	tiva_spi *spi = alloc(sizeof(tiva_spi));  // get 8 bytes for struct

	volatile void *ssi_base;
	volatile void *gpio_base;
	uint8_t gpio_port;
	uint8_t gpio_pinmask, gpio_pmcx;

	switch(ssi_num) {
		case 0:
			ssi_base  = SSI0_BASE;
			gpio_base = GPIOA_BASE;
			gpio_port = GPIO_A;
			gpio_pinmask = 0x3C;
			gpio_pmcx = 2;
			break;
		case 1:
			ssi_base  = SSI1_BASE;
			gpio_base = GPIOD_BASE;
			gpio_port = GPIO_D;
			gpio_pinmask = 0x0F;
			gpio_pmcx = 2;
			break;
		case 2:
			ssi_base  = SSI2_BASE;
			gpio_base = GPIOB_BASE;
			gpio_port = GPIO_B;
			gpio_pinmask = 0xF0;
			gpio_pmcx = 2;
			break;
		case 3:
			ssi_base  = SSI3_BASE;
			gpio_base = GPIOD_BASE;
			gpio_port = GPIO_D;
			gpio_pinmask = 0x0F;
			gpio_pmcx = 1;
			break;
	}

	// Enable specified SSI module via RCGCSSI
	*(volatile uint32_t *)RCGCSSI |= (1 << ssi_num);

	// Activate GPIO pins associated with specied spi/ssi module
	// For now, just activate all associated pins, even if unused
	gpio_init(gpio_port);
	gpio_set_digital_pins(gpio_port, gpio_pinmask);
	gpio_set_altfn_pins(gpio_port, gpio_pinmask);
	gpio_set_fn_pins(gpio_port, gpio_pinmask, gpio_pmcx);

	// config & activate SSI module
	*(volatile uint32_t *)(ssi_base + SSICR1) &= (0x02) ^ (0xFFFFFFFF); /* disable ssi */
	*(volatile uint32_t *)(ssi_base + SSICR1) &= (0x04) ^ (0xFFFFFFFF); /* set as master */
	*(volatile uint32_t *)(ssi_base + SSICC) &= (0x01) ^ (0xFFFFFFFF); /*set to use system clock*/
	*(volatile uint8_t  *)(ssi_base + SSICPSR)  = 0x04; /* set clk prescale*/
	*(volatile uint8_t  *)(ssi_base + SSICR0)  = 0x07; /* set polarity & datawidth */
	*(volatile uint8_t  *)(ssi_base + SSICR1) |= (0x02); /* enable ssi */

	// once sucessfully init'd, set up a return spi struct
	spi->ssi_base = ssi_base;
	spi->gpio_base = gpio_base;
	return spi;
}

// block until ssi module is idle (and ready to transmit)
void spi_txwait(tiva_spi *spi)
{
	while ((*(volatile uint8_t *)(spi->ssi_base + SSISR) & 0x10) != 0)
		;

	return;
}
