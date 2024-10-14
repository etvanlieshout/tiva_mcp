/* process.h
 * Header file for process definition
 */

#ifndef	_PROCESS_H
#define	_PROCESS_H

#define NUMPROC	10 // max number of processes
#define PRI_MAX	10
#define PRI_BOUND (PRI_MAX + 1) // used as limiting bound
#define MCPCTRL	5  // number of switches before scheduler forces main MCP
		   // process to run

/* process states */
#define	P_FREE	0
#define P_CURR	1
#define P_READY	2

struct process {
	int	pid;
	int	state;
	int	priority;
	int	stack_size;
	char	name[16];
	void	*curr_stkptr;
	void	*base_stkptr;
	void	*curr_heap;
	void	*heap_base;
	void 	*proc_pc;
};

struct process	process_table[NUMPROC];
int	curr_pid;
int	process_count;
int	free_pid; // next free pid for new processes
int	reschedule_count;
void	*contxt_sw(void *old_stk, void *new_stk);
int create(
		void	*startaddr,  //start addr of process code (ie fn addr)
		int	stk_size,
		int	priority,
		char	*name,
		int	argc,        // number of args to be passed to start fn
		...
	  );
void	proc_exit();
int	kill(int pid);

#endif
