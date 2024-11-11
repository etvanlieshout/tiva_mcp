/* ===========================================================================*\
 * init.c
 *
 * Contains code that sets up the process table, ready queue, and process memory
 * spaces; then, starts the main Tiva MCP process.
 *
 * The main MCP process begins execution at mcp_main() in the mcp_main.c file.
 * User code should begin within this file & function.
 *
 * Contents:
 * mcp_init()
 * mem_init()
 * sched_timer_init()
 * start_mcp_proc()
 * svc_handler()
 *============================================================================*/

#include <mcp.h>

/* -- mcp_init -----------------------------------------------------------------
 * Initializes memory spaces, process table and ready queue, and then creates
 * and starts the main mcp process.
 * */
void mcp_init()
{
	// set up memory
	mem_init();

	// set schedular globals to initial values
	free_pid = 0;
	process_count = 0;
	curr_pid = 0;
	reschedule_count = 0;

	// Initialize ready Q:
	q_init(READYQ);

	// Create the main mcp process
	char *proc_name = "mcp_base_proc";
	create(&mcp_main, 512, PRI_MAX, proc_name, 0); // set lowest priority

	// switch to PSP & init PSP to MSP-8 for balance.
	// The -8 is used to avoid inadvertent stack smashing within svc_handler.
	psp_init(msp_get()-8);

	mcp_start(); // execution will never return to here
}


/* -- mem_init() ---------------------------------------------------------------
 * sets up the general process memory space
 */
void mem_init()
{
	sys_stk_base = init_usrmem();
	nxt_stk_base = sys_stk_base;
}

/* -- sched_timer_init() -------------------------------------------------------
 * Sets up SysTick clock on Tiva MCU to function as a timer interrupt & thereby
 * be the primary trigger for rescheduling.
 */
void sched_timer_init()
{
	/* Set SysTick Interrupt Priority to 1 (lower priority than SVC & PendSV) */
	/* NOTE: Not used right now */
	/* *(volatile uint32_t *) (COREBASE + SYSPRI3) |= 0x20000000; */

	/* Timer interval: SysTick counts down every main clock cycle; main clock on
	 * the Tiva tm4c123gh6pm is 80MHz. */
	*(volatile uint32_t *) (COREBASE + STRELOAD) |= 800000; /* 0.01s period */

	/* Congfigure & Enable SysTick timer: Select sys clk, Enable Interrupt */
	*(volatile uint8_t *) (COREBASE + STCTRL) |= 0x7;
}

/* -- start_mcp_proc() ---------------------------------------------------------
 * Called by the SVC handler to start base_mcp_proc's main mcp_run control flow.
 * Sets systems PSP to point to the main_mcp_proc's stack. Pops top 8 words of
 * initial stack; handler return will pop remaining process stack into context.
 */
void start_mcp_proc()
{
	sched_timer_init();
	psp_set((&process_table[0])->curr_stkptr); // mcp_proc always has pid 0
	vreg_restore();
}

/* -- SVC Handler ---------------------------------------------------------
 * Handler for SVC. Extracts immediate value and takes action accordingly.
 */
void svc_handler()
{
	uint8_t svc_code = svc_code_get();

	switch (svc_code) {
		case 0xFF:
			start_mcp_proc();
			break;
		case 0xE1:
			break;
		default:
			break;
	}
}
