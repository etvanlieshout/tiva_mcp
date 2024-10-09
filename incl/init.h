/* init.h
 * Header for init
 * */

#ifndef _INIT_H
#define _INIT_H

void mcp_init();
void mem_init();
void mcp_run();
void sched_timer_init();
void *init_usrmem();
void start_mcp_proc();

extern void psp_set(void *sp);
extern void msp_get();
//extern void start_mcp_proc();

#endif
