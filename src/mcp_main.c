/* ===========================================================================*\
 * mcp_main.c
 *
 * mcp_main is the effective main function when using the tiva mcp. User code
 * should start in this file. User code can be structured either as a linear
 * flow that simply begins from mcp_main(), or multiple processes can be created
 * here, with their code either below mcp_main in this file or in a separate
 * file.
 *
 * Contents:
 * mcp_main()
 *
 * NOTE: mcp_main() declaration in init.h
 */

#include <mcp.h>
#include <spi.h>
#include <alice_1502_lcd.h>

// these functions are in led.c and included just for testing
extern void red();
extern void blue();
extern void led_init();

/* -- mcp_main -----------------------------------------------------------------
 * Primary mcp process code begins here.
 * */
void mcp_main()
{
	/* Start of user code */
	tiva_spi *spi2 = spi_init(2);
	lcd_procmon_start(spi2);

	/* Red and Blue processes included for testing */
	char* red_proc = "red";
	int red_pid = create(&red, 512, 1, red_proc, 0);
	char* blue_proc = "blue";
	int blu_pid = create(&blue, 512, 1, blue_proc, 0);
	

	/* keep main mcp_proc alive + bookkeeping, &c. */

	uint32_t a = 1; // for testing process kill
	uint8_t  procmon_count = 0; // for testing procmon lcd
	while(1) {

		/* Update fun process monitor lcd display */
		if (procmon_count - process_count) { // if not equal
			lcd_procmon_update(spi2, (uint8_t)process_count);
			procmon_count = process_count;
		}

		/* Test kill() when called by mcp */
		a++;
		if (a == 3277000 && process_table[red_pid].state != P_FREE) {
			kill(red_pid);
			*(volatile uint8_t *)(0x40025008) = 0; // shutoff led
		}
	}
}
