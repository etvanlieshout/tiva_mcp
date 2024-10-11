/* reschedule.c
 * reschedule() - gives cpu to next process by selecting next process, updating
 * any system variables, and calling context switch.
 * ASSUMES INTERRUPTS DISABLED.
 */

#include <mcp.h>

/* this is a simple scheduler that just pulls from the ready queue and swaps
 * processes if the head of the ready queue has equally or greater priority
 * than the currently running process. ASSUMES interrupts disabled */
void reschedule()
{
	struct process *old_p, *new_p;
	int cpsr_state;

	/* diable interrupts */
	//cpsr_state = disable_i();

	old_p = &process_table[curr_pid];

	// check if current process remains eligible for cpu
	if (old_p->state == P_CURR){
		//if ((old_p->priority < queue_table[QFIRST(READYQ)].priority) &&
		//	(queue_table[QFIRST(READYQ)].pid < QHEAD(READYQ))) {
		if (old_p->priority < queue_table[QFIRST(READYQ)].priority &&
			reschedule_count++ < MCPCTRL) {
			return; // current process still has highest priority
		}
		old_p->state = P_READY; // current process moved to ready queue
		q_insert(READYQ, old_p->pid, old_p->priority);
	}

	// get new process:
	// check if mcp needs to run (b/c it is low priority)
	if (reschedule_count == MCPCTRL) {
		new_p = &process_table[0];
		reschedule_count = 0;
	}
	else {
		// get head of ready queue and make it the new current process
		new_p = &process_table[dequeue(READYQ)];
	}
	new_p->state = P_CURR;
	curr_pid = new_p->pid;

	//preempt = QUANTUM; //reset reschedule interrupt timer

	contxt_sw(&old_p->curr_stkptr, &new_p->curr_stkptr);

	//restore_i(cpsr_state);

	// when old_p next resumes, it will pick up here
	return;
}

void	resched_timer_isr()
{
	// clear interrupt
	*(volatile uint32_t *)(0x40030024) |= 0x1;

	// blinks an LED to verify irq
	//*(volatile uint8_t *)(0x40025010) ^= 0x04;

	// disable interrupts
	int cpsr_state = disable_i();

	reschedule();

	// restore and enable interrupts
	restore_i(cpsr_state);
	enable_i();
	return;
}

