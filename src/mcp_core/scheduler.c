/* ===========================================================================*\
 * reschedule.c
 *
 * Contains scheduler code
 *
 * Contents:
 * reschedule()
 * resched_timer_isr()
 *============================================================================*/

#include <mcp.h>

/* -- Reschedule() -------------------------------------------------------------
 * This is a simple scheduler that just pulls from the ready queue and swaps
 * processes if the head of the ready queue has equally or greater priority
 * than the currently running process. To ensure that the main mcp control flow
 * gets cpu time, a counter tracks reschdule calls and after a certain number
 * of calls the schedule ignores the ready queue and gives the cpu to the main
 * mcp process.
 *
 * ARGS: None
 * Return: None
 *
 * NOTES:
 * ASSUMES INTERRUPTS ARE DISABLED; Should only be called from within a syscall
 */
void reschedule()
{
	struct process *old_p, *new_p;

	++reschedule_count;

	old_p = &process_table[curr_pid];

	/* check if current process remains eligible for cpu */
	if (old_p->state == P_CURR){
		if (reschedule_count < MCPCTRL &&
			old_p->priority < queue_table[QFIRST(READYQ)].priority) {
			return; // current process still has highest priority
		}
		old_p->state = P_READY;
		q_insert(READYQ, old_p->pid, old_p->priority);
	}

	// get new process:
	// check if mcp needs to run (b/c it is always lowest priority)
	if (reschedule_count >= MCPCTRL) {
		new_p = &process_table[0];
		reschedule_count = 0;
		q_remove(READYQ, 0); // remove instead of dequeue
	}
	else {
		// get head of ready queue and make it the new current process
		new_p = &process_table[dequeue(READYQ)];
	}
	new_p->state = P_CURR;
	curr_pid = new_p->pid;

	contxt_sw(&old_p->curr_stkptr, &new_p->curr_stkptr);


	/* when old_p next resumes, it will pick up here */
	return;
}

/* -- resched_timer_isr --------------------------------------------------------
 * Interrupt handler for timer irq. Timer irq triggers at a set interval and 
 * calls the scheduler logic via reschedule()
 *
 * NOTES:
 * Because the ARM interrupt process only preserves r0-r3 & r12, this code
 * also calls two helpers to preesrve the r4-r11.
 */
void	resched_timer_isr()
{
	// clear interrupt
	*(volatile uint32_t *)(0x40030024) |= 0x1;

	// disable interrupts (not necessary if only called as interrupt handler)
	int cpsr_state = disable_i();

	vreg_preserve();
	reschedule();
	vreg_restore();

	// restore and enable interrupts
	restore_i(cpsr_state);
	enable_i();

	return;
}

