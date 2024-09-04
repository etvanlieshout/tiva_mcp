/*
 * Interrupt control
 * Header for interrupt control functions
 * */

#ifndef _IRQ_H
#define _IRQ_H

void    enable_i();
int     disable_i();
void    restore_i(int);

#endif
