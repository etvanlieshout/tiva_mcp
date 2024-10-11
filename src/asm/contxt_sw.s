;===============================================================================
;	context switch
;===============================================================================

	.text
	.global contxt_sw

; -- Context Switch ------------------------------------------------------------
; Swaps the active process stack pointers (PSP)
contxt_sw:
	MRS	r2, PSP
	STR	r2, [r0]
	LDR	r2, [r1]
	MSR	PSP, r2
	MOV	pc, lr

