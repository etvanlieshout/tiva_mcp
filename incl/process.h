/* process.h
 * Header file for process definition
 */

#ifndef	_PROCESS_H
#define	_PROCESS_H

#define NUMPROC	10 // max number of processes

/* process states */
#define	P_FREE	0
#define P_CURR	1
#define P_READY	2

struct process {
	int	pid;
	int	state;
	int	priority;
	char	name[16];
	void	*curr_stkptr;
	void	*base_stkptr;
	int	stack_size;
	void 	*proc_pc;
};

struct process	process_table[NUMPROC];
int	curr_pid;
int	process_count;
int	free_pid; // next free pid for new processes
void	*contxt_sw(void *old_stk, void *new_stk);
int create(
		void	*startaddr,  //start addr of process code (ie fn addr)
		int	stk_size,
		int	priority,
		char	*name,
		int	argc,        // number of args to be passed to start fn
		...
	  );
//void    reschedule();

#endif
