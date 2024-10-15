/* ===========================================================================*\
 * alice_1502_lcd.c
 *
 * functions to use the 1502 lcd screen ONLY in the alice daughter board
 * configuration.
 *
 * INCOMPLETE (only implements what I've needed so far)
 *
 *
 * CONTENTS:
 *============================================================================*/

#include <alice_1502_lcd.h>

void lcd_procmon_start(tiva_spi *spi)
{
	char *s_row1 = "ACTIVE PROCESS";
	char *s_row2 = "COUNT: 00";
	char line2[] = { 0xC0, 0x00 }; // force cursor to start of line 2

	lcd1502_init(spi);
	lcd1502_spi_send(spi, s_row1, LCD_DATA);
	lcd1502_spi_send(spi, line2, LCD_CMD);
	lcd1502_spi_send(spi, s_row2, LCD_DATA);
}

// this assumes: 0 <= pcount < 10
void lcd_procmon_update(tiva_spi *spi, uint8_t pcount)
{
	char cursor_dec = { 0x04, 0x00 }; // shift cursor one place left
	char num = { (pcount + '0'), 0x00 };
	lcd1502_spi_send(spi, cursor_dec, LCD_CMD);
	lcd1502_spi_send(spi, num, LCD_DATA);
}

void lcd1502_init(tiva_spi *spi)
{
	char init_cmds[] = { 0x33, 0x32, // Set lcd to 4-bit mode
					     0x28,       // Set 2 lines, 5x8 px chars
					     0x01,       // Clear display
					     0x06,       // incr cursor after char printed
					     0x0C, 0x00};// display on, hide curosr

	// turn on gpio C to control latch for shift register used by ALICE board
	gpio_init(GPIO_C);
	gpio_set_output_pins(GPIO_C, 0x40);
	gpio_set_digital_pins(GPIO_C, 0x40);
	*((volatile uint8_t *)(GPIOC_BASE + 0x100)) = 0; // write a zero to latch to hold it low

	//lcd_cmd_send2(init_cmds);
	lcd1502_spi_send(spi, init_cmds, LCD_CMD);

	return;
}

// general function that takes a spi struct, string, a type (cmd or data),hmmm...
// data should be null-terminated; since 0x0 is not valid byte for 1602, there
// should not be any issue with that.
void lcd1502_spi_send(tiva_spi *spi, char* data, int dtype)
{
	uint8_t len = 0;
	uint16_t lentmp = 0;

	while (data[len] != 0x00)
		++len;   // this len will be the human count, not index
	// write command bytes as [nibbles to go over spi] to lcdcbuf buffer:
	uint16_t buflen = len * 7;
	char *spibuf = alloc(buflen);
	spibuf[buflen] = 0x0;

	while(lentmp < len) {
		// each nibble sent as 2 bytes, upper sent first
		uint8_t uppern = data[lentmp] & 0xF0;
		uint8_t lowern = (data[lentmp] & 0x0F) << 4;
		uint16_t i = lentmp * 6;
		if (0 == dtype){ // lcd cmd
			spibuf[i++] = uppern;
			spibuf[i++] = uppern | 0x2;
			spibuf[i++] = uppern;
			spibuf[i++] = lowern;
			spibuf[i++] = lowern | 0x2;
			spibuf[i]   = lowern;
		}
		else { // char data to display
			spibuf[i++] = uppern | 0x1;
			spibuf[i++] = uppern | 0x3;
			spibuf[i++] = uppern & 0xF1;  // should really be & 0xF1
			spibuf[i++] = lowern | 0x1;
			spibuf[i++] = lowern | 0x3;
			spibuf[i]   = lowern & 0xF1;
		}
		lentmp++;
	}
	// send
	lentmp = 0;  // reuse as counter
	while(lentmp < buflen) {
		hct595_outdis();
		spi_txwait(spi);
		*((volatile uint32_t *)(spi->ssi_base + SSIDATA)) = spibuf[lentmp++];
		spi_txwait(spi);
		hct595_outen();
		lcd_delay();
	}

	return;
}

// for use with alice daughter board lcd config
// Disables output for hct595 shift reg by writing 0 to its output enable
// puts outputs in high impedence state
static void hct595_outdis()
{
	*((uint8_t *)(GPIOC_BASE + 0x100)) &= ~0x40;
}

// for use with alice daughter board lcd config
// Enables output for hct595 shift reg by writing 1 to its output enable
static void hct595_outen()
{
	*((uint8_t *)(GPIOC_BASE + 0x100)) |= 0x40;
}

/* short delay to give 1502 lcd time to respond */
static void lcd_delay()
{
	uint8_t count = 25;
	while (count--)
		;

	return;
}