/* ===========================================================================*\
 * gpio.c
 *
 * gpio functions
 *
 * INCOMPLETE (only implements what I've needed so far)
 *
 *
 * CONTENTS:
 * gpio_init()
 * gpio_clk_init() [static]
 * gpio_set_output_pins
 * gpio_set_input_pins
 *============================================================================*/

#include <gpio.h>

/* just a wrapper for gpio_clk_init b/c idk I like it this way */
/* PLUS: it takes 3 clk cycles for clock to be enabled for gpio ports, and the
 * nexted returns will eat up (at least 2 of) those cycles*/
void gpio_init(uint8_t port)
{
	void *base = get_base(port);

	if (base) {
		gpio_clk_init(port);
	}
}

static void gpio_clk_init(uint8_t port)
{
	*(volatile uint32_t *)(RCGCGPIO) |= port;
	return;
}

/* Set bits specified in pinmask to 1 to set pins as outputs.
 * Pinmask is an 8-bit bitfield wherein 1 sets a pin to the desired
 * functionality and 0 is ignored. */
void gpio_set_output_pins(uint8_t port, uint8_t pinmask)
{
	void *base = get_base(port);

	if (base) {
		*(volatile uint8_t *)(base + GPIODIR) |= pinmask;
	}
}

/* Set bits specified in pinmask to 0 to set pins as inputs.
 * Pinmask is an 8-bit bitfield wherein 1 sets a pin to the desired
 * functionality and 0 is ignored. */
void gpio_set_input_pins(uint8_t port, uint8_t pinmask)
{
	void *base = get_base(port);

	if (base) {
		*(volatile uint8_t *)(base + GPIODIR) &= ~pinmask;
	}
}

void gpio_set_digital_pins(uint8_t port, uint8_t pinmask)
{
	void *base = get_base(port);

	if (base) {
		*(volatile uint8_t *)(base + GPIODEN) |= pinmask;
	}
}

void gpio_set_altfn_pins(uint8_t port, uint8_t pinmask)
{
	void *base = get_base(port);

	if (base) {
		*(volatile uint8_t *)(base + GPIOAFSEL) |= pinmask;
	}
}

void gpio_set_fn_pins(uint8_t port, uint8_t mask, uint8_t pmcx_enc)
{
	void *base = get_base(port);

	if (!base) { // if base == 0, then abort
		return;
	}

	uint32_t writeback = 0x0;
	uint32_t bitclear = 0x0;
	uint8_t bitget = 128;
	uint8_t bitn   = 7;
	while (bitget) {
		if (bitget & mask)
			writeback |= pmcx_enc << (bitn * 4);
		else
			bitclear   |= 0xF << (bitn << 2);
		bitget = bitget >> 1;
		bitn--;
	}
	*(volatile uint32_t *)(base + GPIOPCTL) &= bitclear;
	*(volatile uint32_t *)(base + GPIOPCTL) |= writeback;
}

/* should not be called outside this file */
static void *get_base(uint8_t port)
{
	switch(port) {
		case GPIO_A:
			return GPIOA_BASE;
		case GPIO_B:
			return GPIOB_BASE;
		case GPIO_C:
			return GPIOC_BASE;
		case GPIO_D:
			return GPIOD_BASE;
		case GPIO_E:
			return GPIOE_BASE;
		case GPIO_F:
			return GPIOF_BASE;
		default:
			return 0;
	}
}
