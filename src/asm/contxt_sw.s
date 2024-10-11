;===============================================================================
; context switch
;===============================================================================

	.text
	.global contxt_sw
	.global restore_i

contxt_sw:
	;PUSH	{r0-r12, lr}
	;PUSH	{lr}            ; WTF IS THIS ??? WHY PUSH TWICE?
	;MRS	r2, XPSR
	;PUSH	{r2}
	MRS	r2, PSP
	STR	r2, [r0]
	LDR	r2, [r1]
	MSR	PSP, r2
	;POP	{r0}
	;BL	restore_i
	;POP	{lr}
	;POP	{r0-r12, lr}
	MOV	pc, lr

