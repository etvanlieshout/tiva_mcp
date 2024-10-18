/* reschedule.h
 * header for resecheduling functions
 * */

#ifndef _RESCHED_H
#define _RESCHED_H

/* globals for watchpoints*/
uint32_t old_psp;
uint32_t new_psp;

void	reschedule();
void    resched_timer_isr(void);

#endif
