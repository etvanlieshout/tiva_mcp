/* stack.c
 * all stack manipulation functions used by create & kill, etc to set up
 * process stacks
 */

#include <mcp.h>

// assumes interrupts disabled, and that size is a multiple of 8
void *new_stack(int size)
{
	void	*new_stk_ptr = nxt_stk_base; // holds addr for new stack
	nxt_stk_base = nxt_stk_base - size;  // void ptr, so stride == 1
	return new_stk_ptr;
}
