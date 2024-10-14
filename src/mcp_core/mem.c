/* ===========================================================================*\
 * mem.c
 *
 * Memory allocation related functions
 *
 * CONTENTS:
 * alloc()
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
 * own entry in the process table. (still should probably disable them, since
 * later I will probably give the mcp process the ability to meddle in other
 * processes' process table entries?)
 *
 * WARNING: Allocations have no metadata and no way to free allocations; given
 * the desired simplicity, I may add a heap rest function that allows users to
 * reset their heap and then copy only desired allocations back into the heap.
 * User would be responsible for not overwriting data in the process of copying
 * desired allocations to lower space on the reset heap. (I am lazy)
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
 * Manage your own damn heap.
 * Resets process's curr_heap ptr to its heap_base. Pointers to previous
 * allcotions remain valid, and so users can copy those allocations they want to
 * keep back to the reset heap space.
 */
void *heap_reset()
{
	uint32_t cspr_restore = disable_i();

	void *ret = 0x0;

	struct process *proc_data = &process_table[curr_pid];

	proc_data->curr_heap = proc_data->heap_base;

	restore_i(cspr_restore);
	enable_i();
	return ret;
}

/* -- alloc copy ---------------------------------------------------------------
 * Manage your own damn heap.
 * Copies an allocation to process's curr_heap pointer; when using, call on
 * lower heap allocations first. Suggested use: create an array of allocation
 * pointers (on stack) for allocations you want to keep, sort them lowest to
 * highest pointer value, then call heap_reset(), then call alloc_copy for each
 * allocation pointer in sorted list.
 */
void alloc_copy(void *p)
{
	uint32_t cspr_restore = disable_i();

	struct process *proc_data = &process_table[curr_pid];
	uint32_t size = *(uint32_t *)(p-4) + 8;
	p -= 8;

	char *dest = proc_data->curr_heap;

	while(size--) {
		*dest++ = *(char *)p++;
	}

	restore_i(cspr_restore);
	enable_i();
}
