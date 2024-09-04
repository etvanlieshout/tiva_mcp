; rescheduling timer interrupt subroutine
;	.text
;	.global resched_timer_isr
;	.global disable_i
;	.global restore_i
;	.global reschedule
;
;resched_timer_isr:
;
;	PUSH {lr, r4-r11}
;
;	MOV r0, #0x0000				; Clear the interrupt
;	MOVT r0, #0x4003			;
;	LDR r1, [r0, #0x024]			;
;	ORR r1, #0x01				;
;	STR r1, [r0, #0x024]			;
;
;	BL	disable_i
;	PUSH	{r0}      ;push CPSR to stack for restore
;
;	; HANDLER FUNCTIONALITY
;	; ALSO MIGHT NEED TO DISABLE, RESET, and RE-ENABLE TIMER
;	BL	reschedule
;
;	POP	{r0}
;	BL	restore_i
;
;	POP {lr, r4-r11}			; POP & Return
;	BX lr					;
