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
		*name_entry++ = *name;
	}
	name_entry = '\0';
	
	// set up new stack
	p_ptr->base_stkptr = new_stack(stk_size); // new syscall to write
	p_ptr->curr_stkptr = p_ptr->base_stkptr;

	// put process on ready queue
	q_insert(READYQ, pid, p_ptr->priority);

	restore_i(cspr_restore);
	return pid;
}

// TODO
int	kill(pid)
{
	return 0;
}