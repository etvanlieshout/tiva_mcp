/* reschedule.h
 * header for resecheduling functions
 * */

#ifndef _STACK_H
#define _STACK_H

void	*new_stack(int size);

void	*sys_stk_base; // OS / process space stk base, set on OS init
void	*nxt_stk_base; // NOTE: sys_stk_base set on init() to PSP

#endif
