/* init.c
 * Functions that set up the process table, scheduler queue, and process memory spaces
 */

#include <mcp.h>

extern void red();
extern void blue();
extern void led_init();


/* set up scheduler timer & add this as main process */
void mcp_init()
{
	// set up memory
	mem_init();

	// set schedular globals to initial values
	free_pid = 0;
	process_count = 0;
	curr_pid = 0;

	// turn on on-board (blue) led to verify reschedule irq
	//led_init();

	// First, create a main process; this one will do nothing
	char *proc_name = "mcp_base_proc";
	create(&mcp_run, 256, 2, proc_name, 0); // set lowest priority
	// switch to PSP
	psp_set((&process_table[0])->curr_stkptr); // mcp_proc always has pid 0

	svc_call();

	//sched_timer_init();
	while (1)
		; // keeps program alive until reschedule timer triggers
}

void mcp_run()
{
	sched_timer_init();

	// run led_init, the create 2 processes: red & blue
	// OR: run led_init as a process and kill it (harder)
	// OR: put led_init inside red() & blue() (easy)

	char* red_proc = "red";
	create(&red, 512, 1, red_proc, 0);
	char* blue_proc = "blue";
	create(&blue, 512, 1, blue_proc, 0);
	
	while (1) {
		;	// can add book-keeping an other things here later
			// could have it print creent proc name to debug term?
	}
}

/* sets up the general process memory space */
void mem_init()
{
	sys_stk_base = init_usrmem();
	nxt_stk_base = sys_stk_base;
}

void sched_timer_init()
{
	// code adapted from 379 library subroutine timer_interrupt_init
	*(volatile uint8_t *) (0x400FE604) |= 0x01;  // turn on Timer 0
	int i=0;
	for (i; i<2; ++i); // delay ??
	*(volatile uint8_t *) (0x4003000C) &= 0xFE;  // clear bit => disable
	*(volatile uint8_t *) (0x40030000) &= 0xFC;  // set to 32-bit mode
	*(volatile uint8_t *) (0x40030004) &= 0xFE;  // set to periodic mode:
	*(volatile uint8_t *) (0x40030004) |= 0x02;  // write 0b10 to low bits
	//*(volatile uint32_t *)(0x40030028)  = 0x1E8480; // 2M = 1/8 sec period
	*(volatile uint32_t *)(0x40030028)  = 0x7A1200; // 8M = 1/2 sec period
	*(volatile uint8_t *) (0x40030018) |= 0x01;  // enable mcu interrupt
	*(volatile uint32_t *)(0xE000E100) |= 0x80000; //mcu accept tmr intrupt
	*(volatile uint8_t *) (0x4003000C) |= 0x01;  // enable timer
}

void start_mcp_proc()
{
	// switch to PSP
	//psp_set((&process_table[0])->curr_stkptr); // mcp_proc always has pid 0
	contxt_sw(msp_get,&((&process_table[0])->curr_stkptr));
}
