;===============================================================================
;	Interrupt & status assembly code
;
; Subroutines for enabling, disabling, and restoring interrupts & CPSR
;===============================================================================


	.text

	.global enable_i
	.global disable_i
	.global restore_i

enable_i:
	cpsie	i
	mov 	pc, lr

; returns current cpsr in r0
disable_i:
	mrs 	r0, XPSR  ; copy cpsr to r0 to return cpsr
	cpsid	i
	mov 	pc, lr

; restores interrupts/CPSR to state given by arg in r0
restore_i:
	PUSH	{r1, r2} ; not strictly necessary?
	MRS	r1, XPSR
	MOV	r2, #0x0220
	MOVT	r2, #0x01F0
	AND	r1, r1, r2
	BIC	r0, r0, r2
	ORR	r1, r1, r0
	MSR	XPSR, r1
	;MSR	XPSR_cfsx, r1 ; alternate
	POP	{r1, r2}
	MOV	pc, lr

