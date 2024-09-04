;===============================================================================
; context switch
;===============================================================================

	.text
	.global contxt_sw
	.global restore_i

contxt_sw:
	PUSH	{r0-r11, lr}
	PUSH	{lr}
	MRS	r2, XPSR
	PUSH	{r2}
	STR	sp, [r0]
	LDR	sp, [r1]
	POP	{r0}
	BL	restore_i
	POP	{lr}
	POP	{r0-r12}
	MOV	pc, r12
