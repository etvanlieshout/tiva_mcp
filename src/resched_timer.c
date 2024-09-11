/* resched_timer.c
 * rescheduling timer interrupt subroutine
 */

#include <mcp.h>

//extern void resched_timer_isr(void);
/*
extern  void	resched_timer_isr()
{
	// disable interrupts
	int cpsr_state = disable_i();
	// base address to clear timer interrupt
	int *temp_addr = (int *)0x00004027;
	int temp = *temp_addr;
	temp = temp | 0x01;
	*temp_addr = temp;

	reschedule();
	// MIGHT ALSO NEED TO DISABLE, RESET, & RE-ENABLE TIMER

	restore_i(cpsr_state);
}
*/
/*
; OLD FOR REFERENCE
; rescheduling timer interrupt subroutine
	.text
	.global resched_timer_isr

resched_timer_isr:

	PUSH {lr, r4-r11}

	MOV r0, #0x0000				; Clear the interrupt
	MOVT r0, #0x4003			;
	LDR r1, [r0, #0x024]			;
	ORR r1, #0x01				;
	STR r1, [r0, #0x024]			;

	BL	disable_i
	PUSH	{r0}      ;push CPSR to stack for restore

	; HANDLER FUNCTIONALITY 
	; ALSO MIGHT NEED TO DISABLE, RESET, and RE-ENABLE TIMER
	BL	reschedule

	POP	{r0}
	BL	restore

	POP {lr, r4-r11}			; POP & Return
	BX lr					;

REWRITE IN C:
void	resched_timer_isr()
{
	// disable interrupts
	int cpsr_state = disable_i();
	// base address to clear timer interrupt
	int *temp_addr = 0x00004027;
	int temp = *temp_addr;
	temp = temp | 0x01;
	*temp_addr = temp;

	reschedule();

	restore_i(cpsr_state);
}
*/