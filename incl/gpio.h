/* gpio.h
 *
 * header file for gpio.c
 */


#ifndef _GPIO_H
#define _GPIO_H

#include <mcp.h>

#define GPIO_A	0x01
#define GPIO_B	0x02
#define GPIO_C	0x04
#define GPIO_D	0x08
#define GPIO_E	0x10
#define GPIO_F	0x20

#define GPIOA_BASE	0x40004000
#define GPIOB_BASE	0x40005000
#define GPIOC_BASE	0x40006000
#define GPIOD_BASE	0x40007000
#define GPIOE_BASE	0x40024000
#define GPIOF_BASE	0x40025000

#define GPIODIR   	0x400
#define	GPIOAFSEL 	0x420
#define	GPIODEN  	0x51C
#define	GPIOPCTL  	0x52C

// indecisiveness
#define SYSCTRL_BASE	0x400FE000
#define	RCGCGPIO_OFF  	0x608 // GPIO Clock Gate Control register offset
#define RCGCGPIO   	0x400FE608

//typedef struct tiva_gpio *tiva_gpio;
typedef	struct tiva_gpio {
	volatile void	*base;
};


void gpio_init(uint8_t port);
static void gpio_clk_init(uint8_t port);
void gpio_set_output_pins(uint8_t port, uint8_t pinmask);
void gpio_set_input_pins(uint8_t port, uint8_t pinmask);
void gpio_set_digital_pins(uint8_t port, uint8_t pinmask);
void gpio_set_altfn_pins(uint8_t port, uint8_t pinmask);
void gpio_set_fn_pins(uint8_t port, uint8_t pinmask, uint8_t pmcx_enc);
static void *get_base(uint8_t port);



#endif
