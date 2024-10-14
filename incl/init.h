/* init.h
 * Header for init
 * */

#ifndef _INIT_H
#define _INIT_H

void mcp_init();
void mem_init();
void mcp_main();
void sched_timer_init();
void *init_usrmem();
void start_mcp_proc();
void svc_handler();

extern void psp_init(void *sp);
extern void psp_set(void *sp);
extern void *msp_get();
extern void mcp_start();
extern uint8_t svc_code_get();
//extern void mcp_svc_test();

#endif
