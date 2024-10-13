/*
 * Code for the processes to blink the LED
 * */
#include <stdint.h>

#define DELAY_R 400000
#define DELAY_B 300000

void blue();
void red();

void blue()
{
	led_init();
	uint8_t led_state = 0;
	uint8_t counter = 77;

	// Test allocator
	uint32_t *ptr1 = alloc(24);
	uint32_t *ptr2 = alloc(16);
	*ptr2     = 0x66666666;
	*(ptr2+1) = 0x99999999;
	*(ptr2+2) = 0x66666666;
	*(ptr2+3) = 0x99999999;

	while (counter--) {
	//while (1) {
		led_state ^= 0x4;
		*(volatile uint8_t *)(0x40025010) = led_state; // toggle pin 1 (red)
		// NOTE: 0x010 offset writes only to blue pin
		int i=0;
		for (i; i < DELAY_B; ++i)
			;
	}
	*(volatile uint8_t *)(0x40025010) = 0; // shutoff

	// test heap rest + manual heap management
	heap_reset();
	alloc_copy(ptr2);

	return;
}

void red()
{
	led_init();
	uint8_t led_state = 0;

	while (1) {
		led_state ^= 0x2;
		*(volatile uint8_t *)(0x40025008) = led_state; // toggle pin 1 (red)
		// NOTE: 0x008 offset writes only to blue pin
		int i=0; // Ti compiler can't do assignments inside for()
		for (i; i < DELAY_R; ++i)
			;
	}
	return;
}

void led_init()
{
	/* gpio port clock init */
	/* gpio base: 40025000 (port F) */
	*(volatile uint8_t *)(0x400FE608) |= 0x20; //sysctl_rcgc_en
	// port clock takes 3 cycles to start, so kill a moment
	int i=0; // Ti compiler can't do assignments inside for()
	for (i; i<1; ++i)
		;

	// set input and digital (done in order);
	// onboard LED gpio pinmask: 0xF -- SHOULDN'T THIS BE 0xE ???
	*(volatile uint8_t *)(0x40025400) |= 0x0E; // set bits to 0
	*(volatile uint8_t *)(0x4002551C) |= 0x0E; // set bits to 1
	return;
}
