/* ===========================================================================*\
 * stack.c
 *
 * Stack manipulation functions used by syscall create() to set up new process
 * stack spaces.
 *
 * CONTENTS:
 * stack()
 */

#include <mcp.h>

/* -- new_stack ----------------------------------------------------------------
 * Get new stack of specified size for a process.
 *
 * ARGS: int size: Requested size of new stack in bytes.
 * Return: void * pointer to new stack base addr
 *
 * NOTES:
 * Assumes interrupts disabled, and that size is valid.
 */
void *new_stack(int size)
{
	void	*new_stk_ptr = nxt_stk_base; // holds addr for new stack
	nxt_stk_base = nxt_stk_base - size;  // void ptr, so stride == 1
	return new_stk_ptr;
}
