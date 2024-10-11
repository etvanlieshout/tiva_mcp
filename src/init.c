/* ===========================================================================*\
 * init.c
 *
 * Contains code that sets up the process table, ready queue, and process memory
 * spaces; then, starts the main Tiva MCP process.
 *
 * The main MCP process begins execution at mcp_run(). User code should begin
 * within this function.
 *
 * Contents:
 * mcp_run()
 * mcp_init()
 * mem_init()
 * sched_timer_init()
 * start_mcp_proc() [Unused]
 */

#include <mcp.h>

// these functions are in led.c and included just for testing
extern void red();
extern void blue();
extern void led_init();


/* -- mcp_run ------------------------------------------------------------------
 * Primary mcp process code begins here. Effectively main() within the mcp.
 * */
void mcp_run()
{
	sched_timer_init();

	/* Start of user code */
	/* Red and Blue processes included for testing */
	char* red_proc = "red";
	create(&red, 512, 1, red_proc, 0);
	char* blue_proc = "blue";
	create(&blue, 512, 1, blue_proc, 0);
	
	/* keep main mcp_proc alive */
	while (1) {
		;	// can add book-keeping an other things here later.
			// eg could have it print current proc name to debug term?
	}
}

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

	// First, create a main process; this one will do nothing
	char *proc_name = "mcp_base_proc";
	create(&mcp_run, 256, 2, proc_name, 0); // set lowest priority
	// switch to PSP (Should this also reset MSP? I'm thinking no, but...)
	psp_set((&process_table[0])->curr_stkptr); // mcp_proc always has pid 0

	svc_call();

	while (1)
		; // keeps program alive until reschedule timer triggers
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
 * Sets up Timer 0 module on Tiva MCU to function as an interrupt and thereby
 * be the primary trigger for rescheduling.
 * TODO: Switch to using systick
 */
void sched_timer_init()
{
	*(volatile uint8_t *) (0x400FE604) |= 0x01;  // turn on/ enable Timer 0
	/* Need to wait 3 clock cycles for Timer 0 module to be enabled */
	int i=0;
	for (i; i<2; ++i);

	/* Configure Timer 0 to be our scheduler interrupt */
	*(volatile uint8_t *) (0x4003000C) &= 0xFE;  // clear bit => disable
	*(volatile uint8_t *) (0x40030000) &= 0xFC;  // set to 32-bit mode
	*(volatile uint8_t *) (0x40030004) &= 0xFE;  // set to periodic mode:
	*(volatile uint8_t *) (0x40030004) |= 0x02;  // write 0b10 to low bits
	//*(volatile uint32_t *)(0x40030028)  = 0x27100; // 160,000 = 0.01s period
	*(volatile uint32_t *)(0x40030028)  = 0x1E8480; // 2M = 1/8 sec period
	//*(volatile uint32_t *)(0x40030028)  = 0x7A1200; // 8M = 1/2 sec period
	*(volatile uint8_t *) (0x40030018) |= 0x01;  // enable mcu interrupt
	*(volatile uint32_t *)(0xE000E100) |= 0x80000; //mcu accept tmr intrupt
	*(volatile uint8_t *) (0x4003000C) |= 0x01;  // enable timer
}

/* -- start_mcp_proc() ---------------------------------------------------------
 * Sets systems PSP to point to the main_mcp_proc's stack via contxt_sw. Called
 * by the SVC handler to start base_mcp_proc's main mcp_run control flow.
 */
void start_mcp_proc()
{
	// Update to just use the same pointer in both args (makes more sense)
	contxt_sw(msp_get,&((&process_table[0])->curr_stkptr));
}
