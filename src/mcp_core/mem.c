/* ===========================================================================*\
 * mem.c
 *
 * Memory allocation related functions
 *
 * The provided allocation tools are bare-bones. The programmer is required to
 * keep track of their allocations and reset the heap when necessary, clearing
 * out no-longer-needed allocations and using alloc_copy to preserve allocations
 * still in use.
 * Manage your own damn heap :)
 *
 * CONTENTS:
 * alloc()
 * heap_reset()
 * alloc_copy()
 */

#include <mcp.h>

/* -- alloc --------------------------------------------------------------------
 * Get heap allocation from within process's memory space
 *
 * ARGS: int size: Requested size of new stack in bytes.
 * Return: void * pointer to new stack base addr
 *
 * NOTES:
 * No need to disable interrupts since calling process will only manipulate its
 * own entry in the process table? (Still chose to disable them, since later I
 * will probably give the mcp process the ability to meddle in other processes'
 * process table entries?)
 */
void *alloc(uint16_t size)
{
	uint32_t cspr_restore = disable_i();

	void *ret = 0x0;

	struct process *proc_data = &process_table[curr_pid];

	if (proc_data->curr_heap + size + 8 < proc_data->curr_stkptr) {
		ret = proc_data->curr_heap + 8;
		proc_data->curr_heap = ret + size + 8;
		*(uint32_t *)(ret - 4) = size;
	}

	restore_i(cspr_restore);
	enable_i();

	return ret;
}

/* -- heap reset ---------------------------------------------------------------
 * Resets process's curr_heap ptr to its heap_base. Pointers to previous
 * allcotions remain valid, allowing users can copy those allocations they want
 * to keep back to the reset heap space.
 */
void heap_reset()
{
	uint32_t cspr_restore = disable_i();

	struct process *proc_data = &process_table[curr_pid];

	proc_data->curr_heap = proc_data->heap_base;

	restore_i(cspr_restore);
	enable_i();
}

/* -- alloc copy ---------------------------------------------------------------
 * Copies an allocation to process's curr_heap pointer; when using, call on
 * lower heap allocations first. Suggested use: sort ALL of a process's
 * allocation pointers lowest to highest addr, then call heap_reset(), then
 * call alloc_copy for each allocation pointer in sorted list. Returns the
 * updated allocation pointer.
 */
void *alloc_copy(void *p)
{
	uint32_t cspr_restore = disable_i();

	struct process *proc_data = &process_table[curr_pid];
	uint32_t size = *(uint32_t *)(p-4) + 8;
	p -= 8;

	char *dest = proc_data->curr_heap;
	void *ret  = dest + 8;
	proc_data->curr_heap += size;

	while(size--) {
		*dest++ = *(char *)p++;
	}

	restore_i(cspr_restore);
	enable_i();

	return ret;
}
