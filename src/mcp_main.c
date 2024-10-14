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
	/* Red and Blue processes included for testing */
	char* red_proc = "red";
	create(&red, 512, 1, red_proc, 0);
	char* blue_proc = "blue";
	create(&blue, 512, 1, blue_proc, 0);
	
	/* keep main mcp_proc alive */
	uint16_t a = 1; // for testing process kill
	uint16_t b = 1;
	while (1) {
		a++;	// can add book-keeping an other things here later.
		if (a == 0) {
			b++; // eg could have it print current proc name to debug term?
			if (b++ == 100)
				kill(1); // kill process pid = 1
		}
		if (b > 100)
			break;
	}
	while(1)
		;
}
