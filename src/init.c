/* init.c
 * Functions that set up the process table, scheduler queue, and process memory spaces
 */

#include <mcp.h>

/* set up scheduler timer & add this as main process */
void multitasking_init()
{
	// First, create a main process; this one will do nothing
	char *proc_name = "os_base_proc";
	create(&os_run, 128, 1, proc_name, 0);
}

/* sets up the general process memory space */
void mem_init()
{
	sys_stk_base = init_usrmem();
	nxt_stk_base = sys_stk_base;
}

void os_run()
{
	while (1) {
		;	// can add book-keeping an other things here later
	}
}

void sched_timer_init()
{
	// code adapted from 379 library subroutine timer_interrupt_init
	*(volatile uint8_t *) (0x400FE604) |= 0x01;  // turn on Timer 0
	*(volatile uint8_t *) (0x4003000C) &= 0xFE;  // clear bit => disable
	*(volatile uint8_t *) (0x40030000) &= 0xFC;  // set to 32-bit mode
	*(volatile uint8_t *) (0x40030004) &= 0xFE;  // set to periodic mode:
	*(volatile uint8_t *) (0x40030004) |= 0x02;  // write 0b10 to low bits
	*(volatile uint32_t *)(0x40030028)  = 0x1E8480; // 2M = 1/8 sec period
	*(volatile uint8_t *) (0x40030018) |= 0x01;  // enable mcu interrupt
	*(volatile uint32_t *)(0xE000E100) |= 0x80000; //mcu accept tmr intrupt
	*(volatile uint8_t *) (0x4003000C) |= 0x01;  // enable timer
}
