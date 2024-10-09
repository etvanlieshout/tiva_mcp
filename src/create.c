/* create.c
 * create and kill processes with
 * create() and kill()
 */

#include <mcp.h>

// syscall create: creates new process and puts it on the ready list
// THIS FIRST IMPLEMENTATION ASSUMES EVERYTHING IS VALID AND DOESN'T PROCESS ANY
// ARGS GIVEN FOR PROCESS
int create(
		void	*startaddr,  //start addr of process code (ie fn addr)
		int	stk_size,
		int	priority,
		char	*name,
		int	argc,        // number of args to be passed to start fn
		...
	  )
{
	int	cspr_restore;
	struct	process	*p_ptr;
	char	*argv;          // pointer to list of args
	void	**stk_addr;

	cspr_restore = disable_i();

	// TODO
	// check stack size & stk addr are multiples of 8
	// check priority is valid
	
	int pid = free_pid++;
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
	p_ptr->base_stkptr = new_stack(stk_size); // new syscall to write
	p_ptr->curr_stkptr = p_ptr->base_stkptr;

	// need to set up the stack to look like what the context switch code
	// expects; have to push some bogus stuff (+ initial args) and set
	// the location where ctxsw pulls the pc from to the startaddr.
	// In contxt_sw, each frame on stack is 14 words, & pc is located @ 14th
	// word. (so: offset of 13 words).
	//*(volatile uint32_t *)p_ptr->curr_stkptr - 13 = startaddr;

	/* set up inital stack frame */
	/* Format:
	 * First 8 words: IRQ handler push: XPSR, pc, lr, r12, r3-r0
	 * Next 2 words: EXC_RETURN (lr updated on entry to handler, pushed on call
	 * to reschedule) + alignment
	 * Next 5 words: pushed by reschedule();
	 * Last: 14 words: context switch push
	 * TOTAL: 29 words
	 */
	*(volatile uint32_t *)p_ptr->curr_stkptr = 0x00000000; /* xPSR @ irq */
	p_ptr->curr_stkptr -= 4;
	*(volatile uint32_t *)p_ptr->curr_stkptr = startaddr; /* pc @ irq */
	                                                      /* code resumes from
														   * here*/
	p_ptr->curr_stkptr -= 4;
	*(volatile uint32_t *)p_ptr->curr_stkptr = 0x00000009; /* lr @ irq */
	p_ptr->curr_stkptr -= 4;
	*(volatile uint32_t *)p_ptr->curr_stkptr = 0xAAAAAAAA; /* r12 @ irq */
	p_ptr->curr_stkptr -= 16;  // skip r0-r3 (b/c no init values)
	*(volatile uint32_t *)p_ptr->curr_stkptr = 0xBAD06969; /* r0 @ irq */
	// ^ test value

	/* below unneeded if using PSP */
	// *(volatile uint32_t *)p_ptr->curr_stkptr = 0xFFFFFFF9; /* EXC_RETURN */
	//p_ptr->curr_stkptr -= 8;

	//p_ptr->curr_stkptr -= 20;  // skip reschedule space ?
	//p_ptr->curr_stkptr -= 14*4;  // skip registers

	// set process program counter start addr
	//p_ptr->proc_pc = startaddr; // not necessary, but does nothing

	// put process on ready queue
	q_insert(READYQ, pid, p_ptr->priority);

	restore_i(cspr_restore);
	enable_i();
	return pid;
}

// TODO
int	kill(pid)
{
	return 0;
}
