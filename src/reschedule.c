/* reschedule.c
 * reschedule() - gives cpu to next process by selecting next process, updating
 * any system variables, and calling context switch.
 * ASSUMES INTERRUPTS DISABLED.
 */

#include <mcp.h>

/* this is a simple scheduler that just pulls from the ready queue and swaps
 * processes if the head of the ready queue has equally or greater priority
 * than the currently running process. */
void reschedule()
{
	struct process *old_p, *new_p;
	old_p = &process_table[curr_pid];

	// check if current process remains eligible for cpu
	if (old_p->state == P_CURR){
		if (old_p->priority < queue_table[QFIRST(READYQ)].priority){
			return; // current process still has highest priority
		}
		old_p->state = P_READY; // current process moved to ready queue
		q_insert(READYQ, old_p->pid, old_p->priority);
	}

	// get head of ready queue and make it the new current process
	new_p = &process_table[dequeue(READYQ)];
	new_p->state = P_CURR;
	curr_pid = new_p->pid;

	preempt = QUANTUM; //reset reschedule interrupt timer

	contxt_sw(&old_p->curr_stkptr, &new_p->curr_stkptr);

	// when old_p next resumes, it will pick up here
	return;
}

void	resched_timer_isr()
{
	// clear interrupt
	*(volatile uint32_t *)(0x40030024) |= 0x1;

	// blinks an LED to verify irq
	*(volatile uint8_t *)(0x40025010) ^= 0x04;

	// disable interrupts
	int cpsr_state = disable_i();
	// base address to clear timer interrupt; below code seems incorrect
	/*int *temp_addr = (int *)0x00004027;
	int temp = *temp_addr;
	temp = temp | 0x01;
	*temp_addr = temp;*/

	reschedule();
	// MIGHT ALSO NEED TO DISABLE, RESET, & RE-ENABLE TIMER

	// restore and enable interrupts
	restore_i(cpsr_state);
	enable_i();
	return;
}

