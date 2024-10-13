;===============================================================================
;	context switch
;===============================================================================

	.text
	.global contxt_sw
	.global vreg_preserve
	.global vreg_restore

; -- Context Switch ------------------------------------------------------------
; Swaps the active process stack pointers (PSP)
contxt_sw:
	MRS	r2, PSP
	STR	r2, [r0]
	LDR	r2, [r1]
	MSR	PSP, r2
	MOV	pc, lr

; -- VReg Preserve -------------------------------------------------------------
; Preserves r4-r11 at current PSP value. Supplements interrupt entry only
; pushing r0-r3 and r12 to PSP (current sp for interrupted process).
vreg_preserve:
	MRS	r0, PSP
	STMFD	r0!, {r4-r11}
	MSR	PSP, r0
	MOV	pc, lr

; -- VReg Restore --------------------------------------------------------------
; Restores r4-r11 from current PSP value. Supplements interrupt exit only
; popping r0-r3 and r12 from PSP (current sp for interrupted process).
vreg_restore:
	MRS	r0, PSP
	LDMFD	r0!, {r4-r11}
	MSR	PSP, r0
	MOV	pc, lr
