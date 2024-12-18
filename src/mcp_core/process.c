/* ===========================================================================*\
 * process.c
 *
 * create and kill processes with create() and kill()
 *
 * NOTE: Only a bare-bones version of create() has been implemented; kill() not
 * implemented at all.
 *
 * Contents:
 * create() [syscall]
 * kill() [syscall]
 * proc_exit()
 *============================================================================*/

#include <mcp.h>

/* -- create() -----------------------------------------------------------------
 * syscall create: creates new process and puts it on the ready list
 * THIS FIRST IMPLEMENTATION ASSUMES EVERYTHING IS VALID AND DOESN'T PROCESS ANY
 * ARGS GIVEN FOR PROCESS
 * */
int create(
		void	*startaddr,  //start addr of process code (ie fn addr)
		int	stk_size,
		int	priority,
		char	*name,
		int	argc,        // number of args to be passed to start fn (unused)
		...
	  )
{
	int	cspr_restore;       // Var to store processor status register
	struct	process	*p_ptr; // pointer to new process struct in proc table
	char	*argv;          // UNUSED; pointer to list of args
	void	**stk_addr;     // UNUSED

	cspr_restore = disable_i();

	// TODO: Error checking
	
	/* get new pid, adjust process mgmt globals, set up process struct */
	int pid = free_pid++; // needs to be more sophisticated (search proc table)
	++process_count;
	p_ptr = &process_table[pid];

	p_ptr->pid = pid;
	p_ptr->state = P_READY;
	p_ptr->priority = priority;
	char *name_entry = p_ptr->name;
	for ( ; *name != '\0'; ){
		*name_entry++ = *name++;
	}
	name_entry = '\0';
	
	// set up new stack
	p_ptr->base_stkptr = new_stack(stk_size);
	p_ptr->curr_stkptr = p_ptr->base_stkptr;
	p_ptr->heap_base = p_ptr->base_stkptr - (stk_size - 8); // -8 preserves
															// alignment &
															// prevents heap
															// base overlapping
															// next stack
	p_ptr->curr_heap = p_ptr->heap_base;

	/* Set up inital stack frame to look like the expected interrupt and context
	 * frame.
	 * 8-words frame to mimic IRQ handler push: XPSR, pc, lr, r12, r3-r0
	 * TODO: consider r4-r11 after updating rescheduling code to preserve these
	 */

	/* PSR @ irq: set to default reset value */
	*(volatile uint32_t *)p_ptr->curr_stkptr = 0x01000000;
	p_ptr->curr_stkptr -= 4;
	/* pc @ irq: set to start of process code */
	*(volatile uint32_t *)p_ptr->curr_stkptr = startaddr;
	p_ptr->curr_stkptr -= 4;
	/* lr @ irq: initialize to anything */
	*(volatile uint32_t *)p_ptr->curr_stkptr = &proc_exit; /* lr @ irq */
	p_ptr->curr_stkptr -= 4;
	/* r12 @ irq: initialize to anything */
	*(volatile uint32_t *)p_ptr->curr_stkptr = 0xAAAAAAAA; /* r12 @ irq */
	p_ptr->curr_stkptr -= 16;  // skip r0-r3 (b/c no init values)
	/* r3-r0 @ irq: initialize to start function args, if any */
	*(volatile uint32_t *)p_ptr->curr_stkptr = 0xBAD00DAB;
	// ^ test value for r0 to check that frame is correctly loaded

	/* leave space for r4-r11 */
	p_ptr->curr_stkptr -= 32;  // skip r4-r11 (b/c no init values)
	*(volatile uint32_t *)p_ptr->curr_stkptr = 0x1111ABBA;
	// ^ test value for r4 to check that frame is correctly loaded

	// put process on ready queue
	q_insert(READYQ, pid, p_ptr->priority);

	// restore & enable interrupts, then return
	restore_i(cspr_restore);
	enable_i();
	return pid;
}

// wrapper for kill that process can exit to; can send exist status to
// mcp_base_proc (this is future functionality)
void proc_exit()
{
	kill(curr_pid);
	while(1) ;	// spin here until process removed from scheduling on next call
				// to reschedule
}

/* -- Kill ---------------------------------------------------------------------
 * TASKS TODO: Needs to be more bookkeeping with process stack space, so that
 * stacks can be made available again after their process gets killed. More
 * bookkeeping needed generally.
 */
int	kill(pid)
{
	// disable interrupts
	int cspr_restore = disable_i();

	if (pid != curr_pid) {
		q_remove(READYQ, pid);
	}
	(&process_table[pid])->state = P_FREE;
	process_count--;

	// restore & enable interrupts, then return
	restore_i(cspr_restore);
	enable_i();
	return 0;
}
