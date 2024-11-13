/* svc.h
 * */

#ifndef _SVC_H
#define _SVC_H


void systick_isr();
void svc_handler();

void _set_pendsv();

extern uint8_t svc_code_get();

#endif
